#pragma once
#include "NetBuffer.h"
#include "KraFight/Detail/Pointer.h"
#include "INetSerialize.h"
#include "HasFunc.h"
#include <type_traits>

namespace kra {
	// Wrapper for NetBuffer class used to exclusively load things from the buffer
	class NetSaveBuffer {
	public:
		NetSaveBuffer(NetBuffer& Buff);

		// Save a value into the buffer
		template<typename T>
		void CopyRaw(T& Val)
		{
			Buffer.CopyTo(LoadIndex, Val);
			LoadIndex += sizeof(T);
		}

		// Save a value into the buffer
		template<typename T>
		NetSaveBuffer& operator<<(T& Val);

		// Save a pointer into the buffer
		template<typename T>
		NetSaveBuffer& operator<<(Pointer<T>& Val)
		{
			static_assert(std::is_base_of_v<INetSerialize, T>, "T is not INetSerialize type, and thus cannot be serialized");

			// Get and save the creation function pointer
			Function<Pointer<INetSerialize>(void)> CPtr = Val->GetCreateFunc();
			Buffer.CopyTo(LoadIndex, CPtr);
			LoadIndex += sizeof(CPtr);

			Val->NetSave(*this);

			return *this;
		}

	private:
		NetBuffer& Buffer;
		size_t LoadIndex;
	};

	// Compile time if statement
	template<typename T, bool B>
	struct RunNetSave;

	template<typename T>
	struct RunNetSave<T, false> {
		static void Func(NetSaveBuffer& Buff, T& Val)
		{
			Buff.CopyRaw(Val);
		}
	};

	template<typename T>
	struct RunNetSave<T, true> {
		static void Func(NetSaveBuffer& Buff, T& Val)
		{
			Val.NetSave(Buff);
		}
	};

	template<typename T>
	inline NetSaveBuffer & NetSaveBuffer::operator<<(T & Val)
	{
		RunNetSave<T, ::HasNetSave<T>>::Func(*this, Val);
		return *this;
	}
}