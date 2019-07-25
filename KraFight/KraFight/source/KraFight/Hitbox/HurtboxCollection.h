#pragma once
#include "Hurtbox.h"
#include "HitCollision.h"
#include "KraFight/Detail/Handle.h"
#include <vector>

namespace kra {
	class Entity;
	struct Context;
	class HitboxCollection;

	class HurtboxCollection {
	public:
		HurtboxCollection(int PlayerNum);

		// Check if this hurtbox hits a given hitbox
		HitCollision Collide(const Context& Con, const HitboxCollection& Hit);

		// Needs to be implemented later on
		// Function to set some kind of hit index to prevent a hitbox from hitting twice
		//void RegisterHit()

		// Removes all hurtboxes
		void ClearHurtboxes();

		// Change or add a specific hurtbox 
		void SetHurtbox(size_t I, const Hurtbox& Hit);

		// Get const reference to the hurtboxes
		const std::vector<Hurtbox>& GetContainer() const;

		// Get owning entity handle
		Handle<Entity> GetOwner() const;

		// Get the player number/index
		int GetPlayerNumber() const;

	private:
		std::vector<Hurtbox> Hurtboxes;
		Handle<Entity> Owner;
		int PlayerNumber;
	};
}