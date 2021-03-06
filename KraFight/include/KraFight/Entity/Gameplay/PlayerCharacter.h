#pragma once
#include "PlayerAttributes.h"
#include "AttackProperties.h"
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

		void SetupPlayer(Handle<InputBuffer> Input, Handle<PlayerStateMachine> StateHandle, int PlayerNum, Handle<Entity> OtherPlayer);

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

	private: // Private functions
		void UpdateMovement(kfloat DeltaTime, const Context & Con, Handle<Entity> Self);

	public: // Getters/Setters
		int GetPlayerNumber() const;
		Handle<InputBuffer> GetInputHandle() const;
		const PlayerAttributes& GetPlayerAttributes() const;

		kfloat GetTimer() const;
		void SetTimer(kfloat Value);

		Handle<HurtboxCollection> GetHurtbox() const;
		Handle<HitboxCollection> GetHitbox() const;

		// Returns if the character is on the ground
		bool IsOnGround(const Context& Con) const;

		// Returns the direction the character is facing
		kfloat GetFacingDirection();

	protected:
		Handle<InputBuffer> InputHandle;
		Handle<PlayerStateMachine> StateMachineHandle;
		PlayerAttributes Attributes;
		int PlayerNumber;
		Handle<Entity> OtherPlayer;

		// Value holding the direction the player is facing
		// Is only updated in certain states, for instance:
		// Idle, Walking, Crouching
		kfloat FacingDirection;

	protected: // Attacks
		// The main body for the hurtboxes
		Handle<HurtboxCollection> HurtboxBody;
		Handle<HitboxCollection> HitboxHandle;

		// Currenty Attack Type
		// Int value so that it can be filled with any enum
		AttackProperties CurrentAttackType;

		// Current Attack
		AttackInstance CurrentAttack;

		// Vector with all possible attacks
		std::vector<Handle<Attack>> Attacks;

	protected:
		// Function called from state condition functions
		void StartAttack(int AttackType, AttackPosition PositionContext);

	private: // States
		// The main state timer variable
		kfloat Timer;

	protected: // State functions
		// Movement
		static bool StateIdleToWalk(const Context& Con, Handle<Entity> Hand);
		static bool StateWalkToIdle(const Context& Con, Handle<Entity> Hand);
		static void StateUpdateIdleSlide(const Context& Con, kfloat DeltaTime, Handle<Entity> Hand);

		// Jump
		static bool StateJumpToLand(const Context& Con, Handle<Entity> Hand);
		static bool StateIdleToJumpSquat(const Context& Con, Handle<Entity> Hand);
		static bool StateJumpSquatToJump(const Context& Con, Handle<Entity> Hand);
		static void StateOnEnterJumpSquat(const Context& Con, Handle<Entity> Hand);

		// Attacks
		static bool StateIdleToGroundAttack(const Context& Con, Handle<Entity> Hand);
		static bool StateGroundAttackToStand(const Context& Con, Handle<Entity> Hand);
		static bool StateGroundAttackToCrouch(const Context& Con, Handle<Entity> Hand);
		static void StateOnEnterGroundAttack(const Context& Con, Handle<Entity> Hand);
		static void StateOnLeaveGroundAttack(const Context& Con, Handle<Entity> Hand);
		static bool StateJumpToAirAttack(const Context& Con, Handle<Entity> Hand);
		static bool StateAirAttackToJump(const Context& Con, Handle<Entity> Hand);
		static bool StateAirAttackToLand(const Context& Con, Handle<Entity> Hand);
		static void StateOnEnterAirAttack(const Context& Con, Handle<Entity> Hand);
		static void StateOnLeaveAirAttack(const Context& Con, Handle<Entity> Hand);

		// Hitstun
		static bool StateHitstunToJump(const Context& Con, Handle<Entity> Hand);
		static bool StateHitstunToKnockdown(const Context& Con, Handle<Entity> Hand);
		static bool StateHitstunGroundToIdle(const Context& Con, Handle<Entity> Hand);
		static void StateUpdateHitstunGround(const Context& Con, kfloat DeltaTime, Handle<Entity> Hand);

		// Blocking
		static bool StateIdleToBlock(const Context& Con, Handle<Entity> Hand);
		static bool StateBlockToIdle(const Context& Con, Handle<Entity> Hand);
		static bool StateStandBlockToCrouchBlock(const Context& Con, Handle<Entity> Hand);
		static bool StateCrouchBlockToStandBlock(const Context& Con, Handle<Entity> Hand);

		// Misc active
		static void StateUpdateFacingDirection(const Context& Con, kfloat DeltaTime, Handle<Entity> Hand);
	};
}