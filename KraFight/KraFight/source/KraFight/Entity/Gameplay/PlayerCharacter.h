#pragma once
#include "PlayerAttributes.h"
#include "KraFight/Entity/Entity.h"
#include "KraFight/Detail/Handle.h"
#include "KraFight/Behavior/StateMachineDef.h"

namespace kra {
	class InputBuffer;
	class HitboxCollection;
	class HurtboxCollection;

	class PlayerCharacter : public Entity {
	public:
		PlayerCharacter();

		void SetupPlayer(Handle<InputBuffer> Input, int PlayerNum);

		// Change the size of physics body
		void OnCreated(const EntContext& Con) override;

		// Destroy the hurtbox
		void OnDestroyed(const EntContext& Con) override;

		// Update state machine and the sorts
		void Update(kfloat DeltaTime, const EntContext& Con) override;

		// Setup the states in the state machine
		virtual void SetupStateMachine(PlayerStateMachineSetup& Setup);

		// Function that returns a function pointer that is able to create an object of the derived type
		Function<Pointer<INetSerialize>(void)> GetCreateFunc() override;

		// Function to save the object into the buffer
		void NetSave(NetSaveBuffer& Buff) override;

		// Function to load the object from the buffer
		void NetLoad(NetLoadBuffer& Buff) override;

	protected:
		Handle<InputBuffer> InputHandle;
		PlayerAttributes Attributes;
		int PlayerNumber;

	protected: // Attacks
		// The main body for the hurtboxes
		Handle<HurtboxCollection> HurtboxBody;
		Handle<HitboxCollection> HitboxHandle;

	};
}