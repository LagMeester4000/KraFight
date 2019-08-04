#include "PlayerCharacter.h"
#include "KraFight/Context.h"
#include "KraFight/Input/InputManager.h"
#include "KraFight/Physics/PhysicsManager.h"
#include "KraFight/Physics/PhysicsObject.h"

using namespace kra;

kra::PlayerCharacter::PlayerCharacter()
{
}

void kra::PlayerCharacter::SetupPlayer(Handle<InputBuffer> Input, int PlayerNum)
{
	InputHandle = Input;
	PlayerNumber = PlayerNum;
}

void kra::PlayerCharacter::OnCreated(const EntContext & Con)
{
	Entity::OnCreated(Con);

	auto& Physics = Con.PhysicsObjects->Get(PhysicsBody);
	Physics.SetSize(Vector2{ 50_k, 100_k });
}

void kra::PlayerCharacter::OnDestroyed(const EntContext & Con)
{
	Entity::OnDestroyed(Con);


}

void kra::PlayerCharacter::Update(kfloat DeltaTime, const EntContext & Con)
{
	Entity::Update(DeltaTime, Con);

	auto& Input = Con.Inputs->Get(InputHandle);
	auto& Physics = Con.PhysicsObjects->Get(PhysicsBody);
	
	Vector2 NewVel = Vector2{ Attributes.WalkSpeed * kfloat::CreateFromInt(Input.StickX()), 0_k };
	Physics.SetVelocity(NewVel);
}

void kra::PlayerCharacter::SetupStateMachine(PlayerStateMachineSetup & Setup)
{
}

Function<Pointer<INetSerialize>(void)> kra::PlayerCharacter::GetCreateFunc()
{
	return Function<Pointer<INetSerialize>(void)>();
}

void kra::PlayerCharacter::NetSave(NetSaveBuffer & Buff)
{
	//Entity::NetSave(Buff);
}

void kra::PlayerCharacter::NetLoad(NetLoadBuffer & Buff)
{
	//Entity::NetLoad(Buff);
}
