#pragma once
#include "KraFight/Behavior/TimelineAction.h"
#include "KraFight/Hitbox/Hitbox.h"

namespace kra {
	class ActionModifyHitbox : public TimelineAction {
	public:
		ActionModifyHitbox();

		// Set the new hitbox
		void Activate(const Context& Con, Handle<Entity> Ent) override;

	public: // Vars
		// Index of the hitbox to be modified
		size_t Index;

		// Actual new hitbox value
		Hitbox NewHitbox;
	};
}