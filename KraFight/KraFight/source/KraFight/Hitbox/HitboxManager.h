#pragma once
#include "KraFight/Detail/Manager.h"
#include "HitboxCollection.h"
#include "HurtboxCollection.h"
#include "HitIdGenerator.h"

namespace kra {
	struct Context;

	class HitboxManager {
	public:
		HitboxManager();
		
		// Check for collisions and call OnHit functions
		void Update(const Context& Con);

		// Get a hitbox from the manager
		HitboxCollection& GetHitbox(Handle<HitboxCollection> Handl);

		// Create a new hitbox
		Handle<HitboxCollection> AddHitbox(Handle<Entity> Owner, int PlayerNumber);

		// Get a hurtbox from the manager
		HurtboxCollection& GetHurtbox(Handle<HurtboxCollection> Handl);

		// Create a new hurtbox
		Handle<HurtboxCollection> AddHurtbox(Handle<Entity> Owner, int PlayerNumber);

		// Destroy a hitbox
		void DestroyHitbox(Handle<HitboxCollection> Hand);

		// Destroy a hurtbox
		void DestroyHurtbox(Handle<HurtboxCollection> Hand);

		// Get the container of hitboxes
		// SHOULD BE USED FOR DEBUGGING
		std::vector<Optional<HitboxCollection>>& HitContainer();

		// Get the container of hurtboxes
		// SHOULD BE USED FOR DEBUGGING
		std::vector<Optional<HurtboxCollection>>& HurtContainer();

	private:
		Manager<HitboxCollection, HitboxCollection> Hitboxes;
		Manager<HurtboxCollection, HurtboxCollection> Hurtboxes;
		HitIdGenerator HitGenerator;
	};
}