#pragma once
#include "KraFight/Detail/Allocator/StackAllocator.h"
#include "KraFight/Detail/AnyPtr.h"
#include <assert.h>

namespace kra {
	class HookCustomEventManager {
	public:
		HookCustomEventManager();

		HookCustomEventManager(const HookCustomEventManager& Other) = delete;
		HookCustomEventManager& operator=(const HookCustomEventManager& Other) = delete;

		HookCustomEventManager(HookCustomEventManager&& Other);
		HookCustomEventManager& operator=(HookCustomEventManager&& Other);

	public: // Funcs
		template<typename T>
		void AddEvent(T Event)
		{
			assert(EventArrayTop < EventArraySize);
			T* NewObj = Alloc.AllocateObject<T>();
			*NewObj = Event;
			AnyPtr NewPtr = NewObj;
			EventArray[EventArrayTop] = NewPtr;
			++EventArrayTop;
		}

		// Reset the event manager
		// Removes all the events
		void Reset();
		
		// Returns size of the event array
		size_t Size() const;

		// Returns AnyPtr to an event
		AnyPtr& operator[](size_t I);

	private:
		StackAllocator Alloc;
		StackAllocator::Checkpoint ResetCheck;
		
		AnyPtr* EventArray;
		size_t EventArraySize;
		size_t EventArrayTop;
	};
}