#pragma once
#include "Pointer.h"
#include "Handle.h"
#include "Optional.h"
#include "KraFight/Network/NetSaveBuffer.h"
#include "KraFight/Network/NetLoadBuffer.h"
#include <vector>

namespace kra {
	template<typename T, typename D>
	class ManagerIterator {
	public:
		ManagerIterator(size_t Index, std::vector<Optional<D>>& Vec)
			: CurrentI(Index), Vector(Vec) {}

		bool operator!=(const ManagerIterator<T, D>& Other)
		{
			CurrentI != Other.CurrentI;
		}

		ManagerIterator<T, D>& operator++()
		{
			bool Try = true;
			while (Try)
			{
				CurrentI++;
				if (CurrentI >= Vector.size())
				{
					return;
				}
				else if (Vector[CurrentI].Exists)
				{
					return;
				}
			}
		}

		D* operator->()
		{
			return &Vector[CurrentI].Value;
		}

	private:
		size_t CurrentI;
		std::vector<Optional<D>>& Vector;
	};

	// T = type
	// D = derived/stored type, the type of values stored in the manager
	// Manager class used to manage object and automatically serialize them
	template<typename T, typename D = Pointer<T>>
	class Manager {
	public:
		Manager() {}

		// Get an object given a handle
		D& Get(Handle<T> Handle)
		{
			//TODO: add safety checks?
			return Objects[Handle.GetHandle()].Value;
		}

		// Get an object given a handle
		D& operator[](Handle<T> Handle)
		{
			return Get(Handle);
		}

		// Add a new object to the manager
		Handle<T> Add(D& Val)
		{
			for (size_t I = 0; I < Objects.size(); ++I)
			{
				auto& It = Objects[I];
				if (!It.Exists)
				{
					// New spot has been found
					It.Value = Val;
					It.Exists = true;
					
					// Make the new handle
					Handle<T> Ret;
					Ret.SetHandle((HandleT)I);
					return Ret;
				}
			}

			// Could not find a free spot, make a new spot
			Optional<D> Push;
			Push.Exists = true;
			Push.Value = Val;
			Objects.push_back(Push);

			// Make new handle
			Handle<T> Ret;
			Ret.SetHandle(Objects.size() - 1);
			return Ret;
		}

		void Destroy(Handle<T> Handle)
		{
			Objects[Handle.GetHandle()].Exists = false;
		}

		// Function to save the object into the buffer
		void NetSave(NetSaveBuffer& Buff)
		{
			size_t Size = Objects.size();

			// Save the amount of elements
			Buff << Size;

			// Loop to save each element
			for (size_t I = 0; I < Size; ++I)
			{
				auto& It = Objects[I];
				
				// Save if the object should be loaded/is active
				Buff << It.Exists;

				if (It.Exists)
				{
					// Handling of pointer/raw value is done in the buffer class
					Buff << It.Value;
				}
			}
		}

		// Function to load the object from the buffer
		void NetLoad(NetLoadBuffer& Buff)
		{
			// Set all objects to not exist
			FlipObjectExistance();

			size_t Size;

			// Load the amount of elements
			Buff >> Size;

			// Loop to load each element
			for (size_t I = 0; I < Size; ++I)
			{
				auto& It = Objects[I];

				// Load if the object should be loaded/is active
				Buff >> It.Exists;

				if (It.Exists)
				{
					// Handling of pointer/raw value is done in the buffer class
					Buff >> It.Value;
				}
			}
		}

		// NOTE: these functions have to be written with this casing because the 
		// standard requires it for iterators
		ManagerIterator<T, D> begin()
		{
			return ManagerIterator<T, D>(0, Objects);
		}

		// NOTE: these functions have to be written with this casing because the 
		// standard requires it for iterators
		ManagerIterator<T, D> end()
		{
			return ManagerIterator<T, D>(Objects.size(), Objects);
		}

		// Get a reference to the container
		std::vector<Optional<D>>& Container()
		{
			return Objects;
		}

	protected:
		void FlipObjectExistance()
		{
			for (auto& It : Objects)
			{
				It.Exists = false;
			}
		}

	protected:
		std::vector<Optional<D>> Objects;
	};
}