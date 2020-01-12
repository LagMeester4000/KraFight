#pragma once
#include "KraFight/Detail/Handle.h"
#include <vector>

namespace kra {
	class Entity;

	// Namespace with all the built in hook event types
	namespace hookEvent {
		struct HookCreateEntity {
			Handle<Entity> Ent;
		};
	}
	
	// Class with arrays for each HookEvent
	class HookEvents {
	public:
		// Clear all the arrays
		// Should be called at the very start (or end) of each frame
		void Clear();

		std::vector<hookEvent::HookCreateEntity> OnCreateEntity;
		std::vector<hookEvent::HookCreateEntity> OnDestroyEntity;
	};
}