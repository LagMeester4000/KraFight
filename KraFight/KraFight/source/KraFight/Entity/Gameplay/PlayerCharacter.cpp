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

	//TEMP code
	auto& Input = Con.Inputs->Get(InputHandle);
	auto& Physics = Con.PhysicsObjects->Get(PhysicsBody);
	
	if (Con.StateMachines->Get(StateMachineHandle)->GetCurrentState() != EPlayerStates::Hitstun)
	{
		Vector2 NewVel = Vector2{ Attributes.WalkSpeed * kfloat::makeFromInt(Input.StickX()), 0_k };
		Physics.SetVelocity(NewVel);
	}
}

void kra::PlayerCharacter::SetupStateMachine(PlayerStateMachineSetup & Setup)
{
	Setup.AddCondition(EPlayerStates::Idle, EPlayerStates::Walk, [](const Context& Con, Handle<Entity> Hand) 
	{
		auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
		if (Self)
		{
			auto Input = Con.Inputs->Get(Self->GetInputHandle());
			return Input.StickX() != 0;
		}
		return false;
	});

	// Attack1
	Setup.AddCondition(EPlayerStates::Idle, EPlayerStates::Attack, [](const Context& Con, Handle<Entity> Hand)
	{
		auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
		if (Self)
		{
			auto Input = Con.Inputs->Get(Self->GetInputHandle());
			if (Input.Pressed(&InputFrame::Attack1))
			{
				Input.Consume(&InputFrame::Attack1);
				return true;
			}
		}
		return false;
	});
	Setup.AddCondition(EPlayerStates::Walk, EPlayerStates::Attack, [](const Context& Con, Handle<Entity> Hand)
	{
		auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
		if (Self)
		{
			auto Input = Con.Inputs->Get(Self->GetInputHandle());
			if (Input.Pressed(&InputFrame::Attack1))
			{
				Input.Consume(&InputFrame::Attack1);
				return true;
			}
		}
		return false;
	});
	Setup.AddCondition(EPlayerStates::Attack, EPlayerStates::Idle, [](const Context& Con, Handle<Entity> Hand)
	{
		auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
		if (Self)
		{
			return !Self->CurrentAttack.IsActive();
		}
		return false;
	});
	Setup.AddOnEnter(EPlayerStates::Attack, [](const Context& Con, Handle<Entity> Hand) 
	{
		auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
		if (Self)
		{
			Self->HitboxHandle = Con.Hitboxes->AddHitbox(Hand, Self->GetPlayerNumber());

			//TEST
			Self->CurrentAttackType = BasicAttackTypes::sm;
			Self->CurrentAttack.Activate(Self->Attacks[Self->CurrentAttackType]);
		}
	});
	Setup.AddOnLeave(EPlayerStates::Attack, [](const Context& Con, Handle<Entity> Hand)
	{
		auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
		if (Self)
		{
			Con.Hitboxes->DestroyHitbox(Self->HitboxHandle);
			Self->HitboxHandle.MakeInvalid();
		}
	});
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

int kra::PlayerCharacter::GetPlayerNumber()
{
	return PlayerNumber;
}

Handle<InputBuffer> kra::PlayerCharacter::GetInputHandle()
{
	return InputHandle;
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

bool kra::PlayerCharacter::IsOnGround(const Context & Con) const
{
	auto& Phys = Con.PhysicsObjects->Get(PhysicsBody);
	return Phys.IsOnGround();
}

