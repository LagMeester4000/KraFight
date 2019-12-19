#pragma once

namespace kra {
	class NetBuffer {
	public:
		NetBuffer();
		NetBuffer(const NetBuffer& Other);
		NetBuffer(NetBuffer&& Other);
		~NetBuffer();
		NetBuffer& operator=(const NetBuffer& Other);
		NetBuffer& operator=(NetBuffer&& Other);

		// Copy an object into the buffer
		template<typename T>
		void CopyTo(size_t Index, const T& Val);

		// Copy an object from the buffer into an object
		template<typename T>
		bool CopyFrom(size_t Index, T& Val);

	private:
		// Reallocate the buffer to a new size, copy existing data over
		// Can fail if new allocation size is smaller than current size
		bool Reallocate(size_t NewSize);

		// Reallocate the buffer to a new size, no copying
		void ReallocateRaw(size_t NewSize);

	private:
		char* Memory = nullptr;
		size_t MemSize = 0;

	};

	template<typename T>
	inline void NetBuffer::CopyTo(size_t Index, const T& Val)
	{
		if (Index + sizeof(T) + 1 > MemSize)
		{
			Reallocate((MemSize + 2) * 2);
		}
		T* Pos = (T*)&Memory[Index];
		*Pos = Val;
	}

	template<typename T>
	inline bool NetBuffer::CopyFrom(size_t Index, T& Val)
	{
		if (Index + sizeof(T) + 1 > MemSize)
		{
			return false;
		}
		Val = *((T*)&Memory[Index]);
		return true;
	}
}