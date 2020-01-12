#pragma once
#include "KraFight/Detail/Handle.h"

namespace kra {
	class PhysicsManager;
	class HitboxManager;
	class EntityManager;
	class InputManager;
	class StateMachineManager;
	class ResourceManager;
	class HookManager;

	// Struct holding references to every system used for gameplay
	// Does not have ownership over any of the systems
	struct Context {
		PhysicsManager* PhysicsObjects;
		HitboxManager* Hitboxes;
		EntityManager* Entities;
		InputManager* Inputs;
		StateMachineManager* StateMachines;
		ResourceManager* Resources;
		HookManager* Hook;
	};
}