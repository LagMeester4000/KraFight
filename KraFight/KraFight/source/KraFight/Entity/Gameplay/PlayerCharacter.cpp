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

using namespace kra;

kra::PlayerCharacter::PlayerCharacter()
{
}

void kra::PlayerCharacter::SetupPlayer(Handle<InputBuffer> Input, Handle<PlayerStateMachine> StateMachin, int PlayerNum)
{
	InputHandle = Input;
	StateMachineHandle = StateMachin;
	PlayerNumber = PlayerNum;

	{
		TempAttack[5].Add(&AttackFuncs::SpawnHitboxAir, 
			0,
			Vector2{ 0_k, 0_k },
			Vector2{ 10_k, 10_k },
			10_k,
			10_k,
			45_k,
			10_k);

		TempAttack[10].Add(&AttackFuncs::StopHitbox, 0);
	}

	// Add hardcoded attack resources
	CurrentAttackTimeline.SetActive(false);
	{
		int Index = BasicAttackTypes::sl;
		
		auto NewTimeline = MakePointer<TimelineResource>();
		if (Attacks.size() <= Index)
		{
			Attacks.resize(Index + 2);
		}
		Attacks[Index] = NewTimeline;

		TimelineElement Push;
		auto NewAction = MakePointer<ActionModifyHitbox>();
		NewAction->Index = 0;
		NewAction->NewHitbox.bActive = true;
		NewAction->NewHitbox.Size = { 25_k, 25_k };
		Push.Action = NewAction;
		Push.TimePoint = FrameTime * 5_k;

		auto& Container = NewTimeline->Container();
		Container.push_back(Push);
	}
	{
		int Index = BasicAttackTypes::sm;

		auto NewTimeline = MakePointer<TimelineResource>();
		if (Attacks.size() <= Index)
		{
			Attacks.resize(Index + 2);
		}
		Attacks[Index] = NewTimeline;

		TimelineElement Push;
		auto NewAction = MakePointer<ActionModifyHitbox>();
		NewAction->Index = 0;
		NewAction->NewHitbox.bActive = true;
		NewAction->NewHitbox.Size = { 25_k, 25_k };
		Push.Action = NewAction;
		Push.TimePoint = FrameTime * 15_k;

		auto& Container = NewTimeline->Container();
		Container.push_back(Push);
	}
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
	CurrentAttackTimeline.Update(DeltaTime, Con, Self);

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
			return Input.Pressed(&InputFrame::Attack1);
		}
		return false;
	});
	Setup.AddCondition(EPlayerStates::Attack, EPlayerStates::Idle, [](const Context& Con, Handle<Entity> Hand)
	{
		auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
		if (Self)
		{
			return Self->IsTimerDone();
		}
		return false;
	});
	Setup.AddOnEnter(EPlayerStates::Attack, [](const Context& Con, Handle<Entity> Hand) 
	{
		auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
		if (Self)
		{
			Self->HitboxHandle = Con.Hitboxes->AddHitbox(Hand, Self->GetPlayerNumber());
			//auto& Hit = Con.Hitboxes->GetHitbox(Self->HitboxHandle);
			//Hitbox NewHit;
			//NewHit.bActive = true;
			//NewHit.Position = { 60_k, 0_k };
			//NewHit.Size = { 10_k, 10_k };
			//NewHit.HitProps.bLauncher = true;
			//NewHit.HitProps.LaunchAngle = 45_k;
			//NewHit.HitProps.LaunchSpeed = 500_k;
			//Hit.SetHitbox(0, NewHit);
			//Self->SetTimer(30_k * FrameTime);

			//TEST
			Self->CurrentAttackType = BasicAttackTypes::sm;

			//Self->CurrentAttackTimeline.SetTimelineResource(&*Self->Attacks[Self->CurrentAttackType]);
			//Self->CurrentAttackTimeline.SetActive(true);
			//Self->CurrentAttackTimeline.Reset();
			//
			//Self->SetTimer(30_k * FrameTime);

			Self->SetTimer(kfloat::makeFromInt(Self->TempAttack.Size()) * FrameTime);
		}
	});
	Setup.AddOnLeave(EPlayerStates::Attack, [](const Context& Con, Handle<Entity> Hand)
	{
		auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
		if (Self)
		{
			//auto& Hit = Con.Hitboxes->GetHitbox(Self->HitboxHandle);
			//Hit.ClearHitboxes();
			Con.Hitboxes->DestroyHitbox(Self->HitboxHandle);
			Self->HitboxHandle.MakeInvalid();
			Self->CurrentAttackTimeline.SetActive(false);
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

