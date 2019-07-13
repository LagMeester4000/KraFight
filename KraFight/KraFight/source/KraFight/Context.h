#pragma once

namespace kra {
	class PhysicsManager;
	class HitboxManager;
	class EntityManager;

	// Struct holding references to every system used for gameplay
	struct Context {
		PhysicsManager* PhysicsObjects;
		HitboxManager* Hitboxes;
		EntityManager* Entities;
	};
}