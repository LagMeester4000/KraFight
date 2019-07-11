#pragma once
#include "KraFight/Detail/Manager.h"
#include "PhysicsObject.h"

namespace kra {
	struct Context;

	class PhysicsManager : public Manager<PhysicsObject, PhysicsObject> {
	public:
		PhysicsManager();

		// Update the physics bodies in the world
		void Update(const Context& Con, kfloat DeltaTime);

	private:
		kfloat WallDistance = 2000.f;
	};
}