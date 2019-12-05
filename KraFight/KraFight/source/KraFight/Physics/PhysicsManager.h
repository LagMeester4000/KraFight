#pragma once
#include "KraFight/Detail/Manager.h"
#include "PhysicsObject.h"

namespace kra {
	struct Context;

	class PhysicsManager : public Manager<PhysicsObject, PhysicsObject> {
	public:
		PhysicsManager();

		// Update the physics bodies in the world
		void Update(const Context& Con, kfloat DeltaTime, Handle<PhysicsObject> Player1, Handle<PhysicsObject> Player2);

	private:
		void UpdatePlayers(const Context& Con, Handle<PhysicsObject> Player1, Handle<PhysicsObject> Player2);

	private:
		kfloat WallDistance = 1000_k;
	};
}