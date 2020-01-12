#include "KraFight/Hook/HookCustomEventManager.h"
#include <utility>

using namespace kra;

const static size_t CustomEventMemSize = 10000;
const static size_t CustomEventAmount = 100;

kra::HookCustomEventManager::HookCustomEventManager()
	: Alloc(CustomEventMemSize)
{
	EventArray = Alloc.AllocateArray<AnyPtr>(CustomEventAmount);
	EventArraySize = CustomEventAmount;
	EventArrayTop = 0;
	ResetCheck = Alloc.GetCheckpoint();
}

kra::HookCustomEventManager::HookCustomEventManager(HookCustomEventManager && Other)
	: Alloc(std::move(Other.Alloc))
{
	ResetCheck = Other.ResetCheck;
	EventArray = Other.EventArray;
	EventArraySize = Other.EventArraySize;
	EventArrayTop = Other.EventArrayTop;

	Other.ResetCheck.Top = 0;
	Other.EventArray = nullptr;
	Other.EventArraySize = 0;
	Other.EventArrayTop = 0;
}

HookCustomEventManager & kra::HookCustomEventManager::operator=(HookCustomEventManager && Other)
{
	Alloc = std::move(Other.Alloc);

	ResetCheck = Other.ResetCheck;
	EventArray = Other.EventArray;
	EventArraySize = Other.EventArraySize;
	EventArrayTop = Other.EventArrayTop;

	Other.ResetCheck.Top = 0;
	Other.EventArray = nullptr;
	Other.EventArraySize = 0;
	Other.EventArrayTop = 0;
}

void kra::HookCustomEventManager::Reset()
{
	Alloc.DeallocateToCheckpoint(ResetCheck);
	EventArrayTop = 0;
}

size_t kra::HookCustomEventManager::Size() const
{
	return EventArraySize;
}

AnyPtr & kra::HookCustomEventManager::operator[](size_t I)
{
	assert(I < EventArraySize);
	return EventArray[I];
}
