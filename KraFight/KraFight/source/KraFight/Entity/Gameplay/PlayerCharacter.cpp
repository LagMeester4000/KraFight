#include "PlayerCharacter.h"
#include "KraFight/Context.h"
#include "Essentials.h"
#include "KraFight/Detail/Pointer.h"
#include "KraFight/Consts.h"
#include "KraFight/Behavior/TimelineResource.h"
#include "KraFight/Behavior/TimelineAction.h"
#include "ActionModifyHitbox.h"
#include "ActionModifyHurtbox.h"
#include "AttackTypes.h"
#include "AttackFuncs.h"
#include "KraFight/Behavior/Attack.h"
#include "KraFight/Detail/VectorFunctions.h"

using namespace kra;

kra::PlayerCharacter::PlayerCharacter()
{
}

void kra::PlayerCharacter::SetupPlayer(Handle<InputBuffer> Input, Handle<PlayerStateMachine> StateMachin, int PlayerNum)
{
	InputHandle = Input;
	StateMachineHandle = StateMachin;
	PlayerNumber = PlayerNum;
}

void kra::PlayerCharacter::OnCreated(const Context & Con, Handle<Entity> Self)
{
	Entity::OnCreated(Con, Self);

	auto& Physics = Con.PhysicsObjects->Get(PhysicsBody);
	Physics.SetSize(Vector2{ 50_k, 100_k });
	Physics.SetGravityForce(10_k);

	HurtboxBody = Con.Hitboxes->AddHurtbox(Self, PlayerNumber);
	auto& Hurt = Con.Hitboxes->GetHurtbox(HurtboxBody);
	Hurtbox Body;
	Body.bActive = true;
	Body.Size = Vector2{ 35_k, 80_k };
	Hurt.SetHurtbox(0, Body);

	//HitboxHandle = Con.Hitboxes->AddHitbox(Self, PlayerNumber);

	// Make some dummy resource data 
	{
		size_t Ind = BasicAttackTypes::sm;

		ExtendToFit(Attacks, Ind);
		Attacks[Ind] = Con.Resources->Attacks.MakeResource("Temp_sm");
		Attack& At = *Con.Resources->Attacks.GetResource(Attacks[Ind]);

		// Do actual edits
		At[5].Add(&AttackFuncs::SpawnHitboxAir,
			0,
			Vector2{ 0_k, 0_k },
			Vector2{ 10_k, 10_k },
			10_k,
			10_k,
			45_k,
			1000_k);

		At[10].Add(&AttackFuncs::StopHitbox, 0);
	}
}

void kra::PlayerCharacter::OnDestroyed(const Context & Con, Handle<Entity> Self)
{
	Entity::OnDestroyed(Con, Self);

	Con.Hitboxes->DestroyHitbox(HitboxHandle);
	Con.Hitboxes->DestroyHurtbox(HurtboxBody);
}

void kra::PlayerCharacter::Update(kfloat DeltaTime, const Context & Con, Handle<Entity> Self)
{
	Entity::Update(DeltaTime, Con, Self);

	// Update timer value
	Timer -= DeltaTime;

	// Update the attack timeline
	AttackContext ACon = AttackContext{ Con, (Entity*)this };
	CurrentAttack.Update(DeltaTime, ACon);

	UpdateMovement(DeltaTime, Con, Self);
}

void kra::PlayerCharacter::SetupStateMachine(PlayerStateMachineSetup & Setup)
{
	// Movement
	Setup.AddCondition(EPlayerStates::Idle, EPlayerStates::Walk, &StateIdleToWalk);
	Setup.AddCondition(EPlayerStates::Walk, EPlayerStates::Idle, &StateWalkToIdle);

	// Jump
	Setup.AddCondition(EPlayerStates::Idle, EPlayerStates::JumpSquat, &StateIdleToJumpSquat);
	Setup.AddOnEnter(EPlayerStates::JumpSquat, &StateOnEnterJumpSquat);
	Setup.AddCondition(EPlayerStates::JumpSquat, EPlayerStates::Jump, &StateJumpSquatToJump); 
	Setup.AddCondition(EPlayerStates::Jump, EPlayerStates::Idle, &StateJumpToLand);

	// Attack1
	Setup.AddCondition(EPlayerStates::Idle, EPlayerStates::GroundAttack, &StateIdleToGroundAttack);
	Setup.AddCondition(EPlayerStates::Walk, EPlayerStates::GroundAttack, &StateIdleToGroundAttack);
	Setup.AddCondition(EPlayerStates::GroundAttack, EPlayerStates::Idle, &StateGroundAttackToStand);
	Setup.AddCondition(EPlayerStates::GroundAttack, EPlayerStates::Crouch, &StateGroundAttackToCrouch);
	Setup.AddOnEnter(EPlayerStates::GroundAttack, &StateOnEnterGroundAttack);
	Setup.AddOnLeave(EPlayerStates::GroundAttack, &StateOnLeaveGroundAttack);
}

void kra::PlayerCharacter::OnGetHit(const HitProperties & Hit, Handle<Entity> Other, const Context & Con, Handle<Entity> Self)
{
	auto& Phys = Con.PhysicsObjects->Get(PhysicsBody);

	if (Hit.bLauncher)
	{
		Vector2 Ang;
		Ang.X = kra::cos(Hit.LaunchAngle);
		Ang.Y = kra::sin(Hit.LaunchAngle);
		Ang = Ang * Hit.LaunchSpeed;
		Phys.SetVelocity(Ang);
	}

	Con.StateMachines->Get(StateMachineHandle)->ForceSetState(EPlayerStates::Hitstun, Con);
}

Function<Pointer<INetSerialize>(void)> kra::PlayerCharacter::GetCreateFunc()
{
	INET_FUNCTION(PlayerCharacter);
}

void kra::PlayerCharacter::NetSave(NetSaveBuffer & Buff)
{
	//Entity::NetSave(Buff);
}

void kra::PlayerCharacter::NetLoad(NetLoadBuffer & Buff)
{
	//Entity::NetLoad(Buff);
}

bool kra::PlayerCharacter::IsTimerDone() const
{
	return Timer < 0_k;
}

void kra::PlayerCharacter::UpdateMovement(kfloat DeltaTime, const Context & Con, Handle<Entity> Self)
{
	//TEMP code
	auto& Input = Con.Inputs->Get(InputHandle);
	auto& Physics = Con.PhysicsObjects->Get(PhysicsBody);
	auto CurrentState = Con.StateMachines->Get(StateMachineHandle)->GetCurrentState();

	if (CurrentState != EPlayerStates::Hitstun)
	{
		if (CurrentState == EPlayerStates::Walk)
		{
			Vector2 NewVel = Vector2{ Attributes.WalkSpeed * kfloat::makeFromInt(Input.StickX()), 0_k };
			Physics.SetVelocity(NewVel);
		}

		
	}
}

int kra::PlayerCharacter::GetPlayerNumber() const
{
	return PlayerNumber;
}

Handle<InputBuffer> kra::PlayerCharacter::GetInputHandle() const
{
	return InputHandle;
}

const PlayerAttributes & kra::PlayerCharacter::GetPlayerAttributes() const
{
	return Attributes;
}

kfloat kra::PlayerCharacter::GetTimer() const
{
	return Timer;
}

void kra::PlayerCharacter::SetTimer(kfloat Value)
{
	Timer = Value;
}

int kra::PlayerCharacter::GetCurrentAttackType() const
{
	return CurrentAttackType;
}

void kra::PlayerCharacter::SetCurrentAttackType(int Value)
{
	CurrentAttackType = Value;
}

Handle<HurtboxCollection> kra::PlayerCharacter::GetHurtbox() const
{
	return HurtboxBody;
}

Handle<HitboxCollection> kra::PlayerCharacter::GetHitbox() const
{
	return HitboxHandle;
}

bool kra::PlayerCharacter::IsOnGround(const Context & Con) const
{
	auto& Phys = Con.PhysicsObjects->Get(PhysicsBody);
	return Phys.IsOnGround();
}

bool kra::PlayerCharacter::StateIdleToWalk(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		auto Input = Con.Inputs->Get(Self->GetInputHandle());
		return Input.StickX() != 0;
	}
	return false;
}

bool kra::PlayerCharacter::StateWalkToIdle(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		auto Input = Con.Inputs->Get(Self->GetInputHandle());
		return Input.StickX() == 0;
	}
	return false;
}

bool kra::PlayerCharacter::StateJumpToLand(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		return Con.PhysicsObjects->Get(Self->GetPhysicsBody()).IsOnGround();
	}
	return false;
}

bool kra::PlayerCharacter::StateIdleToJumpSquat(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		auto Input = Con.Inputs->Get(Self->GetInputHandle());
		return Input.StickY() == 1;
	}
	return false;
}

bool kra::PlayerCharacter::StateJumpSquatToJump(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		auto Input = Con.Inputs->Get(Self->GetInputHandle());
		if (Self->IsTimerDone())
		{
			auto& Atts = Self->GetPlayerAttributes();

			kfloat HVelScalar = kfloat::makeFromInt(Input.StickX());
			Vector2 JumpVec;
			JumpVec.X = HVelScalar * Atts.JumpHSpeed;
			JumpVec.Y = Atts.JumpVSpeed;
			Con.PhysicsObjects->Get(Self->GetPhysicsBody()).SetVelocity(JumpVec);

			return true;
		}
	}
	return false;
}

void kra::PlayerCharacter::StateOnEnterJumpSquat(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		Self->SetTimer(FrameTime * 5_k);
	}
}

bool kra::PlayerCharacter::StateIdleToGroundAttack(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		auto Input = Con.Inputs->Get(Self->GetInputHandle());
		if (Input.Pressed(&InputFrame::Attack1))
		{
			Input.Consume(&InputFrame::Attack1);
			Self->CurrentAttackType = BasicAttackTypes::sl;
			return true;
		}
		if (Input.Pressed(&InputFrame::Attack2))
		{
			Input.Consume(&InputFrame::Attack2);
			Self->CurrentAttackType = BasicAttackTypes::sm;
			return true;
		}
		if (Input.Pressed(&InputFrame::Attack3))
		{
			Input.Consume(&InputFrame::Attack3);
			Self->CurrentAttackType = BasicAttackTypes::sh;
			return true;
		}
	}
	return false;
}

bool kra::PlayerCharacter::StateGroundAttackToStand(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		return !Self->CurrentAttack.IsActive();
	}
	return false;
}

bool kra::PlayerCharacter::StateGroundAttackToCrouch(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		return !Self->CurrentAttack.IsActive();
	}
	return false;
}

void kra::PlayerCharacter::StateOnEnterGroundAttack(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		Self->HitboxHandle = Con.Hitboxes->AddHitbox(Hand, Self->GetPlayerNumber());

		//TEST
		//Self->CurrentAttackType = BasicAttackTypes::sm;
		Self->CurrentAttack.Activate(Self->Attacks[Self->CurrentAttackType]);
	}
}

void kra::PlayerCharacter::StateOnLeaveGroundAttack(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		Con.Hitboxes->DestroyHitbox(Self->HitboxHandle);
		Self->HitboxHandle.MakeInvalid();
		Self->CurrentAttack.Deactivate();
	}
}

bool kra::PlayerCharacter::StateJumpToAirAttack(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		auto Input = Con.Inputs->Get(Self->GetInputHandle());
		if (Input.Pressed(&InputFrame::Attack1))
		{
			Input.Consume(&InputFrame::Attack1);
			Self->CurrentAttackType = BasicAttackTypes::jl;
			return true;
		}
		if (Input.Pressed(&InputFrame::Attack2))
		{
			Input.Consume(&InputFrame::Attack2);
			Self->CurrentAttackType = BasicAttackTypes::jm;
			return true;
		}
		if (Input.Pressed(&InputFrame::Attack3))
		{
			Input.Consume(&InputFrame::Attack3);
			Self->CurrentAttackType = BasicAttackTypes::jh;
			return true;
		}
	}
}

bool kra::PlayerCharacter::StateAirAttackToJump(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		return !Self->CurrentAttack.IsActive();
	}
}

bool kra::PlayerCharacter::StateAirAttackToLand(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		return Con.PhysicsObjects->Get(Self->PhysicsBody).IsOnGround();
	}
}

void kra::PlayerCharacter::StateOnEnterAirAttack(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{

	}
}

void kra::PlayerCharacter::StateOnLeaveAirAttack(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		Con.Hitboxes->DestroyHitbox(Self->HitboxHandle);
		Self->HitboxHandle.MakeInvalid();
	}
}

