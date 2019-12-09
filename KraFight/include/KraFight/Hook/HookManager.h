#pragma once
#include "KraFight/Detail/AnyPtr.h"
#include "KraFight/Detail/Pointer.h"
#include "KraFight/Detail/Function.h"
#include "EntityTypeIndex.h"
#include <vector>

namespace kra {
	class IHook;
	struct Context;

	// Class to manage connections between the framework and engine
	class HookManager {
	public:
		HookManager();

		// Get the hook object
		const AnyPtr& GetHook();

		// Register a hook into the HookManager
		// Only register one hook for each gameplay entity type please
		template<typename T>
		void RegisterHook(Pointer<IHook> Pointer)
		{
			size_t TI = EntityTypeIndex::GetTypeIndex<T>();
			if (Hooks.size() <= TI)
			{
				Hooks.resize(TI * 2);
			}
			Hooks[TI] = Pointer;
		}

		void SetCallbackRollback(FunctionRaw<void(const Context& Con, AnyPtr Hook, int CurrentFrame)> Func);

		void SetCallbackSyncUpdate(FunctionRaw<void(const Context& Con, AnyPtr Hook, int CurrentFrame)> Func);

	private:
		// A pointer to an object outside of the framework
		// To be used in callback functions
		AnyPtr Hook;
		
		// All the hooks for the entities
		std::vector<Pointer<IHook>> Hooks;

		/// Callback functions
		// Funtion called when the game is rolled back
		FunctionRaw<void(const Context& Con, AnyPtr Hook, int NewFrame)> CallbackRollback;

		// Function called to update after a rollback to get into the synced state
		FunctionRaw<void(const Context& Con, AnyPtr Hook, int CurrentFrame)> CallbackSyncUpdate;
	};
}