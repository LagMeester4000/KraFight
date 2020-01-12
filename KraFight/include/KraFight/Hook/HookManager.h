#pragma once
#include "KraFight/Detail/AnyPtr.h"
#include "HookEvents.h"
#include "HookCustomEventManager.h"
#include <memory>

namespace kra {
	class IHook;
	struct Context;

	class HookManager {
	public:
		HookManager();

	public: // Event functions
		// Should be called at the very end of the frame
		// Calls the actual hook functions and then clears the event buffers
		void Update(const Context& Con);

		// Register entity for creation
		// NOT TO BE USED OUTSIDE OF THE FRAMEWORK
		void RegisterCreateEntity(Handle<Entity> Ent);
		
		// Registers destruction of entity
		// NOT TO BE USED OUTSIDE OF THE FRAMEWORK
		// If the entity was created was created the same frame, 
		//   it will not be added and the creation event will be destroyed.
		//   You could see this cause issues, but this is exacty how it would work most 
		//   other engines/frameworks. Not doing this would require some weird workarounds.
		void RegisterDestroyEntity(Handle<Entity> Ent);

		// Registere a custom event to be called at the end of the frame
		// This object should not have any dynamic memory (such as std::vector) 
		//   because the object destructor will not be called
		template<typename T>
		void RegisterCustomEvent(T Val)
		{
			CustomEvents.AddEvent(Val);
		}
		
	public: // External object functions
		// Get pointer to external object
		AnyPtr GetExternal() const;

		// Get hook object
		const std::unique_ptr<IHook>& GetHook() const;

		// Set external object
		void SetExternal(AnyPtr Ext);

		// Set hook object
		void SetHook(std::unique_ptr<IHook>&& Hook);

	private:
		// Hook objects to call hook events to
		std::unique_ptr<IHook> Hook;
		
		// Data needed for calling event functions (arguments)
		HookEvents Events;

		// External
		AnyPtr External;

		// Custom events
		HookCustomEventManager CustomEvents;
	};
}