#pragma once
#include <new>

namespace kra {
	class StackAllocator {
	public:
		struct Checkpoint {
			size_t Top;
		};

	public:
		StackAllocator(size_t TotalSize);
		~StackAllocator();

		// Copy
		StackAllocator(const StackAllocator& Other);
		StackAllocator& operator=(const StackAllocator& Other);

		// Move
		StackAllocator(StackAllocator&& Other);
		StackAllocator& operator=(StackAllocator&& Other);

	public: // Funcs
		// Return memory block of size "Size"
		void* Allocate(size_t Size);

		// Deallocates all the memory on the stack
		void DeallocateAll();

		// Get a checkpoint that can be used to deallocate to
		Checkpoint GetCheckpoint() const;

		// Deallocate to the checkpoint
		void DeallocateToCheckpoint(Checkpoint Check);

		// Get raw pointer to memory
		void* GetMemory();

		// Allocate object
		// Keep in mind that destructor of the object 
		//   will not be called when the allocator is cleared
		template<typename T>
		T* AllocateObject()
		{
			void* Mem = Allocate(sizeof(T));
			::new(Mem) T();
			return (T*)Mem;
		}

		// Allocate array
		// Keep in mind that destructor of the object 
		//   will not be called when the allocator is cleared
		template<typename T>
		T* AllocateArray(size_t ArSize)
		{
			void* Mem = Allocate(sizeof(T) * ArSize);
			T* Ret = (T*)Mem;
			for (size_t I = 0; I < ArSize; ++I)
			{
				Ret[I] = T();
			}
			return Ret;
		}

	private:
		void Delete();

	private:
		void* Memory;
		size_t Size;
		size_t Top;
	};
}