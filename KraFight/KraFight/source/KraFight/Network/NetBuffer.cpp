#include "NetBuffer.h"
#include <cstring>

using namespace kra;

kra::NetBuffer::NetBuffer()
{
}

kra::NetBuffer::NetBuffer(const NetBuffer & Other)
{
	ReallocateRaw(Other.MemSize);
	memcpy_s((void*)Memory, Other.MemSize, (void*)Other.Memory, Other.MemSize);
	MemSize = Other.MemSize;
}

kra::NetBuffer::NetBuffer(NetBuffer && Other)
{
	// Remove old memory
	if (Memory)
	{
		delete[] Memory;
	}

	// Use others pointer
	Memory = Other.Memory;
	MemSize = Other.MemSize;

	// Disable other
	Other.Memory = nullptr;
	Other.MemSize = 0;
}

kra::NetBuffer::~NetBuffer()
{
	if (Memory)
	{
		delete[] Memory;
		Memory = nullptr;
	}
	MemSize = 0;
}

NetBuffer & kra::NetBuffer::operator=(const NetBuffer & Other)
{
	ReallocateRaw(Other.MemSize);
	memcpy_s((void*)Memory, Other.MemSize, (void*)Other.Memory, Other.MemSize);
	MemSize = Other.MemSize;

	return *this;
}

NetBuffer & kra::NetBuffer::operator=(NetBuffer && Other)
{
	// Remove old memory
	if (Memory)
	{
		delete[] Memory;
	}

	// Use others pointer
	Memory = Other.Memory;
	MemSize = Other.MemSize;

	// Disable other
	Other.Memory = nullptr;
	Other.MemSize = 0;
	
	return *this;
}

bool kra::NetBuffer::Reallocate(size_t NewSize)
{
	// Cant reallocate smaller memory
	if (NewSize < MemSize)
	{
		return false;
	}

	// Allocate the new data
	char* NewMem = new char[NewSize];
	
	if (Memory)
	{
		// Copy the data
		memcpy_s((void*)NewMem, MemSize, (void*)Memory, MemSize);

		// Delete old memory
		delete[] Memory;
	}

	// Set new vars
	Memory = NewMem;
	MemSize = NewSize;
}

void kra::NetBuffer::ReallocateRaw(size_t NewSize)
{
	if (Memory)
	{
		delete[] Memory;
	}

	Memory = new char[NewSize];
	MemSize = NewSize;
}
