#pragma once
#include "PlayerAttributes.h"
#include "KraFight/Entity/Entity.h"
#include "KraFight/Detail/Handle.h"
#include "KraFight/Behavior/StateMachineDef.h"
#include "KraFight/Behavior/AttackInstance.h"

namespace kra {
	class InputBuffer;
	class HitboxCollection;
	class HurtboxCollection;
	class Attack;

	class PlayerCharacter : public Entity {
	public:
		PlayerCharacter();

		void SetupPlayer(Handle<InputBuffer> Input, Handle<PlayerStateMachine> StateHandle, int PlayerNum);

		// Change the size of physics body
		void OnCreated(const Context& Con, Handle<Entity> Self) override;

		// Destroy the hurtbox
		void OnDestroyed(const Context& Con, Handle<Entity> Self) override;

		// Update state machine and the sorts
		void Update(kfloat DeltaTime, const Context& Con, Handle<Entity> Self) override;

		// Setup the states in the state machine
		virtual void SetupStateMachine(PlayerStateMachineSetup& Setup);

		// Getting hit
		void OnGetHit(const HitProperties& Hit, Handle<Entity> Other, const Context& Con, Handle<Entity> Self) override;

		// Function that returns a function pointer that is able to create an object of the derived type
		Function<Pointer<INetSerialize>(void)> GetCreateFunc() override;

		// Function to save the object into the buffer
		void NetSave(NetSaveBuffer& Buff) override;

		// Function to load the object from the buffer
		void NetLoad(NetLoadBuffer& Buff) override;

	public: // Checks
		// Check if the state timer is done
		bool IsTimerDone() const;

	public: // Getters/Setters
		int GetPlayerNumber();
		Handle<InputBuffer> GetInputHandle();

		kfloat GetTimer() const;
		void SetTimer(kfloat Value);

		int GetCurrentAttackType() const;
		void SetCurrentAttackType(int Value);

		// Returns if the character is on the ground
		bool IsOnGround(const Context& Con) const;

	protected:
		Handle<InputBuffer> InputHandle;
		Handle<PlayerStateMachine> StateMachineHandle;
		PlayerAttributes Attributes;
		int PlayerNumber;

	public: // Attacks
		// The main body for the hurtboxes
		Handle<HurtboxCollection> HurtboxBody;
		Handle<HitboxCollection> HitboxHandle;

		// Currenty Attack Type
		// Int value so that it can be filled with any enum
		int CurrentAttackType;

		// Current Attack
		AttackInstance CurrentAttack;

		// Vector with all possible attacks
		std::vector<Handle<Attack>> Attacks;

	public: // States
		// The main state timer variable
		kfloat Timer;
	};
}