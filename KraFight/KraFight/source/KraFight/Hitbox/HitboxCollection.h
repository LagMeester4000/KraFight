#pragma once
#include "KraFight/Network/NetSaveBuffer.h"
#include "KraFight/Network/NetLoadBuffer.h"
#include "KraFight/Network/SerFuncs/Vector.h"
#include "KraFight/Detail/Handle.h"
#include "HitboxData.h"

namespace kra {
	class Entity;

	// A collection of multiple hitboxes
	// This is used to represent an attack in-game
	class HitboxCollection {
	public:
		HitboxCollection();

		// Removes all hitboxes
		void ClearHitboxes();

		// Change or add a specific hitbox 
		void SetHitbox(size_t I, const HitboxData& Hit);



	private:
		std::vector<HitboxData> Hitboxes;
		Handle<Entity> Owner;
	};
}