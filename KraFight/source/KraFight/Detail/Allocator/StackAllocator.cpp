#include "KraFight/Detail/Allocator/StackAllocator.h"
#include <assert.h>
#include <cstring>
#include <stdint.h>

using namespace kra;

kra::StackAllocator::StackAllocator(size_t TotalSize)
	: Memory(nullptr), Size(TotalSize), Top(0)
{
	Memory = ::operator new(TotalSize);
}

kra::StackAllocator::~StackAllocator()
{
	Delete();
}

kra::StackAllocator::StackAllocator(const StackAllocator & Other)
{
	Memory = ::operator new(Other.Size);
	Size = Other.Size;
	Top = Other.Top;
	std::memcpy(Memory, Other.Memory, Size);
}

StackAllocator & kra::StackAllocator::operator=(const StackAllocator & Other)
{
	Delete();
	Memory = ::operator new(Other.Size);
	Size = Other.Size;
	Top = Other.Top;
	std::memcpy(Memory, Other.Memory, Size);
}

kra::StackAllocator::StackAllocator(StackAllocator && Other)
{
	Memory = Other.Memory;
	Size = Other.Size;
	Top = Other.Top;
	Other.Memory = nullptr;
	Other.Size = 0;
	Other.Top = 0;
}

StackAllocator & kra::StackAllocator::operator=(StackAllocator && Other)
{
	Delete();
	Memory = Other.Memory;
	Size = Other.Size;
	Top = Other.Top;
	Other.Memory = nullptr;
	Other.Size = 0;
	Other.Top = 0;
}

void * kra::StackAllocator::Allocate(size_t MSize)
{
	assert(Top + MSize <= Size);
	void* Ret = (uint8_t*)Memory + Top;
	Top += MSize;
	return Ret;
}

void kra::StackAllocator::DeallocateAll()
{
	Top = 0;
}

StackAllocator::Checkpoint kra::StackAllocator::GetCheckpoint() const
{
	return { Top };
}

void kra::StackAllocator::DeallocateToCheckpoint(Checkpoint Check)
{
	assert(Top >= Check.Top);
	Top = Check.Top;
}

void * kra::StackAllocator::GetMemory()
{
	return Memory;
}

void kra::StackAllocator::Delete()
{
	if (Memory)
	{
		::operator delete(Memory);
	}

	Memory = nullptr;
	Size = 0;
	Top = 0;
}
