#include "PlayerCharacter.h"
#include "KraFight/Context.h"
#include "Essentials.h"
#include "KraFight/Detail/Pointer.h"
#include "KraFight/Consts.h"

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

	HitboxHandle = Con.Hitboxes->AddHitbox(Self, PlayerNumber);
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
			auto& Hit = Con.Hitboxes->GetHitbox(Self->HitboxHandle);
			Hitbox NewHit;
			NewHit.bActive = true;
			NewHit.Position = { 60_k, 0_k };
			NewHit.Size = { 10_k, 10_k };
			NewHit.HitProps.bLauncher = true;
			NewHit.HitProps.LaunchAngle = 45_k;
			NewHit.HitProps.LaunchSpeed = 500_k;
			Hit.SetHitbox(0, NewHit);
			Self->SetTimer(30_k * FrameTime);
		}
	});
	Setup.AddOnLeave(EPlayerStates::Attack, [](const Context& Con, Handle<Entity> Hand)
	{
		auto Self = Con.Entities->Get<PlayerCharacter>(Hand);
		if (Self)
		{
			auto& Hit = Con.Hitboxes->GetHitbox(Self->HitboxHandle);
			Hit.ClearHitboxes();
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
