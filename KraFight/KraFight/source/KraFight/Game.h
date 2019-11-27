#pragma once
#include "TypeDef.h"
#include "Detail/Pointer.h"
#include "Network/NetBuffer.h"
#include "Context.h"

namespace kra {
	struct InputFrame;
	class InputManager;
	class PhysicsManager;
	class HitboxManager;
	class EntityManager;
	class StateMachineManager;

	// Game class which runs and manages the game
	class Game {
	public:
		Game();

		// Update the game systems
		void Update(kfloat DeltaTime, const InputFrame& P1Input, const InputFrame& P2Input);

		// Store the current state
		void StoreState();

		// Restore the stored state
		void RestoreState();

		// Make a context object with the needed systems
		Context MakeContext();

	private:
		Pointer<InputManager> Input;
		Pointer<PhysicsManager> Physics;
		Pointer<HitboxManager> Hitboxes;
		Pointer<EntityManager> Entities;
		Pointer<StateMachineManager> StateMachines;
		Pointer<ResourceManager> Resources;
		NetBuffer StateBuffer;
	};
}