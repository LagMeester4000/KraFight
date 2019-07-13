#pragma once
#include "KraFight/Detail/Manager.h"
#include "HitboxCollection.h"

namespace kra {
	class HitboxManager : private Manager<HitboxCollection, HitboxCollection> {
	public:

		// Get a hitbox from the manager
		HitboxCollection& Get(Handle<HitboxCollection> Handl);

		// Create a new hitbox
		Handle<HitboxCollection> Add(Handle<Entity> Owner);


	};
}