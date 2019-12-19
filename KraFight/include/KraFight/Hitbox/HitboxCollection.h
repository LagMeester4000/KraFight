#pragma once
#include "KraFight/Network/NetSaveBuffer.h"
#include "KraFight/Network/NetLoadBuffer.h"
#include "KraFight/Network/SerFuncs/Vector.h"
#include "KraFight/Detail/Handle.h"
#include "Hitbox.h"
#include "HitId.h"

namespace kra {
	class Entity;
	class NetSaveBuffer;
	class NetLoadBuffer;

	// A collection of multiple hitboxes
	// This is used to represent an attack in-game
	class HitboxCollection {
	public:
		HitboxCollection(Handle<Entity> Own, int PlayerNum, HitId Hit);

		// Removes all hitboxes
		void ClearHitboxes();

		// Change or add a specific hitbox 
		void SetHitbox(size_t I, const Hitbox& Hit);

		// Get const reference to the hitboxes
		const std::vector<Hitbox>& GetContainer() const;

		// Get owning entity handle
		Handle<Entity> GetOwner() const;

		// Get the player number/index
		int GetPlayerNumber() const;

		// Get the HitId
		HitId GetHitId() const;

	public: // Networking
		void NetSave(NetSaveBuffer& Buff);
		void NetLoad(NetLoadBuffer& Buff);

	private:
		std::vector<Hitbox> Hitboxes;
		Handle<Entity> Owner;
		int PlayerNumber = 0;
		HitId Id;
	};
}