#pragma once
#include "TypeDef.h"
#include "Detail/Pointer.h"
#include "Detail/Handle.h"
#include "Network/NetBuffer.h"
#include "Context.h"
#include <memory>

namespace kra {
	struct InputFrame;
	class InputManager;
	class PhysicsManager;
	class HitboxManager;
	class EntityManager;
	class StateMachineManager;
	class HookManager;
	class PlayerCharacter;
	class Entity;

	// Game class which runs and manages the game
	class Game {
	public:
		Game();
		~Game();

		// Update the game systems
		void Update(kfloat DeltaTime, const InputFrame& P1Input, const InputFrame& P2Input);

		// Store the current state
		void StoreState();

		// Restore the stored state
		void RestoreState();

		// Make a context object with the needed systems
		Context MakeContext();

		// Setup both the players in the world
		void SetupPlayers(Pointer<PlayerCharacter> Player1, Pointer<PlayerCharacter> Player2);

		// Get one of the players
		// Can be accessed with either 0 or 1
		Pointer<PlayerCharacter> GetPlayer(int PlayerIndex);
		Handle<Entity> GetPlayerHandle(int PlayerIndex);

	private:
		std::unique_ptr<InputManager> Input;
		std::unique_ptr<PhysicsManager> Physics;
		std::unique_ptr<HitboxManager> Hitboxes;
		std::unique_ptr<EntityManager> Entities;
		std::unique_ptr<StateMachineManager> StateMachines;
		std::unique_ptr<ResourceManager> Resources;
		std::unique_ptr<HookManager> Hook;
		NetBuffer StateBuffer;

		// Players
		Handle<Entity> Player1, Player2;
	};
}