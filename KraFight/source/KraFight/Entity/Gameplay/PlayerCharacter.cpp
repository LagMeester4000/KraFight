#include "KraFight/Entity/Gameplay/PlayerCharacter.h"
#include "KraFight/Context.h"
#include "KraFight/Entity/Gameplay/Essentials.h"
#include "KraFight/Detail/Pointer.h"
#include "KraFight/Consts.h"
#include "KraFight/Behavior/TimelineResource.h"
#include "KraFight/Behavior/TimelineAction.h"
#include "KraFight/Entity/Gameplay/ActionModifyHitbox.h"
#include "KraFight/Entity/Gameplay/ActionModifyHurtbox.h"
#include "KraFight/Entity/Gameplay/AttackTypes.h"
#include "KraFight/Entity/Gameplay/AttackFuncs.h"
#include "KraFight/Behavior/Attack.h"
#include "KraFight/Detail/VectorFunctions.h"
#include "KraFight/Entity/Gameplay/GlobalAttributes.h"
#include "KraFight/Network/NetLoadBuffer.h"
#include "KraFight/Network/NetSaveBuffer.h"
#include "KraFight/Network/SerFuncs/Vector.h"

using namespace kra;

kra::PlayerCharacter::PlayerCharacter()
{
}

void kra::PlayerCharacter::SetupPlayer(Handle<InputBuffer> Input, Handle<PlayerStateMachine> StateMachin, int PlayerNum, Handle<Entity> OtherPlayerr)
{
	InputHandle = Input;
	StateMachineHandle = StateMachin;
	PlayerNumber = PlayerNum;
	OtherPlayer = OtherPlayerr;
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
			Vector2{ 50_k, 0_k },
			Vector2{ 30_k, 30_k },
			10_k,
			10_k,
			80_k,
			400_k);

		At[10].Add(&AttackFuncs::StopHitbox, 0);
	}
	{
		size_t Ind = BasicAttackTypes::sl;

		ExtendToFit(Attacks, Ind);
		Attacks[Ind] = Con.Resources->Attacks.MakeResource("Temp_sl");
		Attack& At = *Con.Resources->Attacks.GetResource(Attacks[Ind]);

		// Do actual edits
		At[5].Add(&AttackFuncs::SpawnHitboxGround,
			0,
			Vector2{ 20_k, -20_k },
			Vector2{ 30_k, 30_k },
			10_k,
			60_k,
			200_k);
		At[7].Add(&AttackFuncs::SpawnHitboxGround,
			0,
			Vector2{ 30_k, -10_k },
			Vector2{ 30_k, 30_k },
			10_k,
			60_k,
			200_k);
		At[9].Add(&AttackFuncs::SpawnHitboxGround,
			0,
			Vector2{ 40_k, 0_k },
			Vector2{ 30_k, 30_k },
			10_k,
			60_k,
			200_k);

		At[11].Add(&AttackFuncs::StopHitbox, 0);
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
	AttackContext ACon = AttackContext{ Con, Self };
	CurrentAttack.Update(DeltaTime, ACon);

	UpdateMovement(DeltaTime, Con, Self);
}

void kra::PlayerCharacter::SetupStateMachine(PlayerStateMachineSetup & Setup)
{
	// Movement
	Setup.AddCondition(EPlayerStates::Idle, EPlayerStates::Walk, &StateIdleToWalk);
	Setup.AddCondition(EPlayerStates::Walk, EPlayerStates::Idle, &StateWalkToIdle);
	Setup.AddUpdate(EPlayerStates::Idle, &StateUpdateFacingDirection);
	Setup.AddUpdate(EPlayerStates::Walk, &StateUpdateFacingDirection);
	Setup.AddUpdate(EPlayerStates::Crouch, &StateUpdateFacingDirection);
	Setup.AddUpdate(EPlayerStates::Idle, &StateUpdateIdleSlide);
	Setup.AddUpdate(EPlayerStates::Crouch, &StateUpdateIdleSlide);
	Setup.AddUpdate(EPlayerStates::GroundAttack, &StateUpdateIdleSlide);
	Setup.AddUpdate(EPlayerStates::BlockLow, &StateUpdateIdleSlide);
	Setup.AddUpdate(EPlayerStates::BlockHigh, &StateUpdateIdleSlide);

	// Jump
	Setup.AddCondition(EPlayerStates::Idle, EPlayerStates::JumpSquat, &StateIdleToJumpSquat);
	Setup.AddCondition(EPlayerStates::Walk, EPlayerStates::JumpSquat, &StateIdleToJumpSquat);
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

	// Hitstun
	Setup.AddCondition(EPlayerStates::Hitstun, EPlayerStates::Jump, &StateHitstunToJump);
	Setup.AddCondition(EPlayerStates::Hitstun, EPlayerStates::Knockdown, &StateHitstunToKnockdown);
	Setup.AddCondition(EPlayerStates::HitstunGround, EPlayerStates::Idle, &StateHitstunGroundToIdle);
	Setup.AddUpdate(EPlayerStates::HitstunGround, &StateUpdateHitstunGround);
}

void kra::PlayerCharacter::OnGetHit(const HitProperties & Hit, Handle<Entity> Other, const Context & Con, Handle<Entity> Self)
{
	auto OtherEnt = Con.Entities->Get(Other);
	auto& Phys = Con.PhysicsObjects->Get(PhysicsBody);
	auto& OtherPhys = Con.PhysicsObjects->Get(OtherEnt->GetPhysicsBody());
	auto& Input = Con.Inputs->Get(InputHandle);
	auto State = Con.StateMachines->Get(StateMachineHandle)->GetCurrentState();
	auto Pos1 = Phys.GetPosition();
	auto Pos2 = OtherPhys.GetPosition();

	// Direction for knockack
	kfloat DirectionS = Pos1.X < Pos2.X ? -1_k : 1_k;

	// Check for blocking
	bool bBlocked = false;
	EPlayerStates NewBlockState;
	if (State == EPlayerStates::Idle ||
		State == EPlayerStates::Crouch ||
		State == EPlayerStates::Walk ||
		State == EPlayerStates::BlockLow ||
		State == EPlayerStates::BlockHigh ||
		State == EPlayerStates::BlockAir)
	{
		if (Pos1.X < Pos2.X)
		{
			if (Hit.Height == HitHeight::Middle || Hit.Height == HitHeight::Low)
			{
				// Need to block low
				if (Input.StickX() == -1 && Input.StickY() == -1)
				{
					// Success
					bBlocked = true;
					NewBlockState = EPlayerStates::BlockLow;
				}
			}
			else if (Hit.Height == HitHeight::Middle || Hit.Height == HitHeight::Overhead)
			{
				// Need to block high
				if (Input.StickX() == -1 && Input.StickY() == 0)
				{
					// Success
					bBlocked = true;
					NewBlockState = EPlayerStates::BlockHigh;
				}
			}
		}
		else
		{
			if (Hit.Height == HitHeight::Middle || Hit.Height == HitHeight::Low)
			{
				// Need to block low
				if (Input.StickX() == 1 && Input.StickY() == -1)
				{
					// Success
					bBlocked = true;
					NewBlockState = EPlayerStates::BlockLow;
				}
			}
			else if (Hit.Height == HitHeight::Middle || Hit.Height == HitHeight::Overhead)
			{
				// Need to block high
				if (Input.StickX() == 1 && Input.StickY() == 0)
				{
					// Success
					bBlocked = true;
					NewBlockState = EPlayerStates::BlockHigh;
				}
			}
		}
	}

	// Block knockback
	if (bBlocked)
	{
		Vector2 NewVel;
		NewVel.X = Hit.GroundKnockbackOnBlock * DirectionS;
		Phys.SetVelocity(NewVel);

		Con.StateMachines->Get(StateMachineHandle)->ForceSetState(NewBlockState, Con);
		SetTimer(Hit.BlockStun * FrameTime);
		return;
	}

	// Apply knockback
	if (Hit.bLauncher || !Phys.IsOnGround())
	{
		Vector2 Ang;
		Ang.X = kra::cos(Hit.LaunchAngle) * DirectionS;
		Ang.Y = kra::sin(Hit.LaunchAngle);
		Ang = Ang * Hit.LaunchSpeed;
		Phys.SetVelocity(Ang);

		// Set hitstun
		Con.StateMachines->Get(StateMachineHandle)->ForceSetState(EPlayerStates::Hitstun, Con);
		SetTimer(Hit.AirHitstun* FrameTime);
	}
	else
	{
		Vector2 NewVel;
		NewVel.X = Hit.GroundKnockback * DirectionS;
		Phys.SetVelocity(NewVel);

		// Set hitstun
		Con.StateMachines->Get(StateMachineHandle)->ForceSetState(EPlayerStates::HitstunGround, Con);
		SetTimer(Hit.GroundHitstun* FrameTime);
	}
}

Function<Pointer<INetSerialize>(void)> kra::PlayerCharacter::GetCreateFunc()
{
	INET_FUNCTION(PlayerCharacter);
}

void kra::PlayerCharacter::NetSave(NetSaveBuffer & Buff)
{
	Entity::NetSave(Buff);
	Buff << InputHandle
		<< StateMachineHandle
		<< Attributes
		<< PlayerNumber
		<< OtherPlayer
		<< FacingDirection
		<< HurtboxBody
		<< HitboxHandle
		<< CurrentAttackType
		<< CurrentAttack
		<< Attacks
		<< Timer;
}

void kra::PlayerCharacter::NetLoad(NetLoadBuffer & Buff)
{
	Entity::NetLoad(Buff);
	Buff >> InputHandle
		>> StateMachineHandle
		>> Attributes
		>> PlayerNumber
		>> OtherPlayer
		>> FacingDirection
		>> HurtboxBody
		>> HitboxHandle
		>> CurrentAttackType
		>> CurrentAttack
		>> Attacks
		>> Timer;
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

kfloat kra::PlayerCharacter::GetFacingDirection()
{
	return FacingDirection;
}

void kra::PlayerCharacter::StartAttack(int AttackType, AttackPosition PositionContext)
{
	CurrentAttackType.AttackType = AttackType;
	CurrentAttackType.Position = PositionContext;
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
		if (Input.StickX() == 0)
		{
			Vector2 NewVel{ 0_k, 0_k };
			Con.PhysicsObjects->Get(Self->PhysicsBody).SetVelocity(NewVel);
			return true;
		}
		return false;
	}
	return false;
}

void kra::PlayerCharacter::StateUpdateIdleSlide(const Context & Con, kfloat DeltaTime, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (!Self)
		return;

	auto& Phys = Con.PhysicsObjects->Get(Self->PhysicsBody);
	auto Vel = Phys.GetVelocity();
	kfloat Sub = GlobalAttributes::Instance.NormalGroundFriction * DeltaTime;

	if (kra::abs(Vel.X) <= Sub)
	{
		Vector2 NewVel{ 0_k, 0_k };
		Phys.SetVelocity(NewVel);
	}
	else if (Vel.X > 0_k)
	{
		Vector2 NewVel{ Vel.X - Sub, 0_k };
		Phys.SetVelocity(NewVel);
	}
	else
	{
		Vector2 NewVel{ Vel.X + Sub, 0_k };
		Phys.SetVelocity(NewVel);
	}
}

bool kra::PlayerCharacter::StateJumpToLand(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (Self)
	{
		auto& Phys = Con.PhysicsObjects->Get(Self->GetPhysicsBody());
		if (Phys.IsOnGround())
		{
			Vector2 NewVel{ 0_k,0_k };
			Phys.SetVelocity(NewVel);
			return true;
		}
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
			//Self->CurrentAttackType = BasicAttackTypes::sl;
			Self->StartAttack(BasicAttackTypes::sl, AttackPosition::Standing);
			return true;
		}
		if (Input.Pressed(&InputFrame::Attack2))
		{
			Input.Consume(&InputFrame::Attack2);
			//Self->CurrentAttackType = BasicAttackTypes::sm;
			Self->StartAttack(BasicAttackTypes::sm, AttackPosition::Standing);
			return true;
		}
		if (Input.Pressed(&InputFrame::Attack3))
		{
			Input.Consume(&InputFrame::Attack3);
			//Self->CurrentAttackType = BasicAttackTypes::sh;
			Self->StartAttack(BasicAttackTypes::sh, AttackPosition::Standing);
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
		Self->CurrentAttack.Activate(Self->Attacks[Self->CurrentAttackType.AttackType]);
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
			//Self->CurrentAttackType = BasicAttackTypes::jl;
			Self->StartAttack(BasicAttackTypes::sl, AttackPosition::Jumping);
			return true;
		}
		if (Input.Pressed(&InputFrame::Attack2))
		{
			Input.Consume(&InputFrame::Attack2);
			//Self->CurrentAttackType = BasicAttackTypes::jm;
			Self->StartAttack(BasicAttackTypes::jm, AttackPosition::Jumping);
			return true;
		}
		if (Input.Pressed(&InputFrame::Attack3))
		{
			Input.Consume(&InputFrame::Attack3);
			//Self->CurrentAttackType = BasicAttackTypes::jh;
			Self->StartAttack(BasicAttackTypes::jh, AttackPosition::Jumping);
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

bool kra::PlayerCharacter::StateHitstunToJump(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (!Self)
		return false;

	return Self->IsTimerDone();
}

bool kra::PlayerCharacter::StateHitstunToKnockdown(const Context & Con, Handle<Entity> Hand)
{
	return false;
}

bool kra::PlayerCharacter::StateHitstunGroundToIdle(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (!Self)
		return false;

	return Self->IsTimerDone();
}

void kra::PlayerCharacter::StateUpdateHitstunGround(const Context & Con, kfloat DeltaTime, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (!Self)
		return;

	auto& Phys = Con.PhysicsObjects->Get(Self->PhysicsBody);
	auto Vel = Phys.GetVelocity();
	kfloat Sub = GlobalAttributes::Instance.HitstunGroundFriction * DeltaTime;

	if (kra::abs(Vel.X) <= Sub)
	{
		Vector2 NewVel { 0_k, 0_k };
		Phys.SetVelocity(NewVel);
	}
	else if (Vel.X > 0_k)
	{
		Vector2 NewVel{ Vel.X - Sub, 0_k };
		Phys.SetVelocity(NewVel);
	}
	else
	{
		Vector2 NewVel{ Vel.X + Sub, 0_k };
		Phys.SetVelocity(NewVel);
	}
}

bool kra::PlayerCharacter::StateIdleToBlock(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (!Self)
		return false;

	auto Other = Con.Entities->Get<PlayerCharacter>(Self->OtherPlayer);
	if (!Other)
		return false;

	// Check controller first (is cheaper)
	auto& Input = Con.Inputs->Get(Self->InputHandle);
	auto& Phys = Con.PhysicsObjects->Get(Self->PhysicsBody);
	auto& OtherPhys = Con.PhysicsObjects->Get(Other->PhysicsBody);
	auto Pos1 = Phys.GetPosition();
	auto Pos2 = OtherPhys.GetPosition();
	if (Pos1.X < Pos2.X)
	{
		if (Input.StickX() != -1)
		{
			return false;
		}
	}
	else
	{
		if (Input.StickX() != 1)
		{
			return false;
		}
	}

	// If there exists a hitbox that is able to hit this player
	const auto& Hits = Con.Hitboxes->HitContainer();
	for (auto& Hit : Hits)
	{
		if (Hit.Exists)
		{
			if (Hit.Value.GetPlayerNumber() != Self->PlayerNumber)
			{
				return true;
			}
		}
	}
	return false;
}

bool kra::PlayerCharacter::StateBlockToIdle(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (!Self)
		return false;

	// Need to stay in blockstun
	if (!Self->IsTimerDone())
		return false;

	auto Other = Con.Entities->Get<PlayerCharacter>(Self->OtherPlayer);
	if (!Other)
		return false;

	// Check controller first (is cheaper)
	auto& Input = Con.Inputs->Get(Self->InputHandle);
	auto& Phys = Con.PhysicsObjects->Get(Self->PhysicsBody);
	auto& OtherPhys = Con.PhysicsObjects->Get(Other->PhysicsBody);
	auto Pos1 = Phys.GetPosition();
	auto Pos2 = OtherPhys.GetPosition();
	if (Pos1.X < Pos2.X)
	{
		if (Input.StickX() != -1)
		{
			return true;
		}
	}
	else
	{
		if (Input.StickX() != 1)
		{
			return true;
		}
	}

	// If there exists a hitbox that is able to hit this player
	const auto& Hits = Con.Hitboxes->HitContainer();
	for (auto& Hit : Hits)
	{
		if (Hit.Exists)
		{
			if (Hit.Value.GetPlayerNumber() != Self->PlayerNumber)
			{
				return false;
			}
		}
	}
	return true;
}

bool kra::PlayerCharacter::StateStandBlockToCrouchBlock(const Context & Con, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (!Self)
		return false;

	

	return false;
}

bool kra::PlayerCharacter::StateCrouchBlockToStandBlock(const Context & Con, Handle<Entity> Hand)
{
	return false;
}

void kra::PlayerCharacter::StateUpdateFacingDirection(const Context & Con, kfloat DeltaTime, Handle<Entity> Hand)
{
	auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
	if (!Self)
		return;

	auto Other = Con.Entities->Get<PlayerCharacter>(Self->OtherPlayer);
	if (!Other)
		return;

	auto& Phys1 = Con.PhysicsObjects->Get(Self->PhysicsBody);
	auto& Phys2 = Con.PhysicsObjects->Get(Other->PhysicsBody);

	if (Phys1.GetPosition().X < Phys2.GetPosition().X)
	{
		Self->FacingDirection = 1_k;
	}
	else
	{
		Self->FacingDirection = -1_k;
	}
}

