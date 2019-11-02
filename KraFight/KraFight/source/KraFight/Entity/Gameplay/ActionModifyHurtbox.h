#pragma once
#include "KraFight/Behavior/TimelineAction.h"
#include "KraFight/Hitbox/Hurtbox.h"

namespace kra {
	class ActionModifyHurtbox : public TimelineAction {
	public:
		ActionModifyHurtbox();

		// Set the new hitbox
		void Activate(const Context& Con, Handle<Entity> Ent) override;

	public: // Vars
		// Index of the hurtbox to be modified
		size_t Index;

		// Actual new hitbox value
		Hurtbox NewHurtbox;
	};
}