#pragma once
#include "KraFight/Detail/Handle.h"

namespace kra {
	class PhysicsManager;
	class HitboxManager;
	class EntityManager;
	class InputManager;
	class StateMachineManager;

	// Struct holding references to every system used for gameplay
	struct Context {
		PhysicsManager* PhysicsObjects;
		HitboxManager* Hitboxes;
		EntityManager* Entities;
		InputManager* Inputs;
		StateMachineManager* StateMachines;
	};
}