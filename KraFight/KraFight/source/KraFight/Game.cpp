#include "Game.h"
#include "Input/InputManager.h"
#include "Physics/PhysicsManager.h"
#include "Hitbox/HitboxManager.h"
#include "Entity/EntityManager.h"
#include "KraFight/Behavior/StateMachineManager.h"
#include "Network/NetSaveBuffer.h"
#include "Network/NetLoadBuffer.h"

using namespace kra;

kra::Game::Game()
{
	Input = MakePointer<InputManager>();
	Physics = MakePointer<PhysicsManager>();
	Hitboxes = MakePointer<HitboxManager>();
	Entities = MakePointer<EntityManager>();
	StateMachines = MakePointer<StateMachineManager>();
}

void kra::Game::Update(kfloat DeltaTime, const InputFrame & P1Input, const InputFrame & P2Input)
{
	Input->Edit(Handle<InputBuffer>(0)) = P1Input;
	Input->Edit(Handle<InputBuffer>(1)) = P2Input;

	auto Con = MakeContext();

	Entities->Update(Con, DeltaTime);
	StateMachines->Update(Con, DeltaTime);
	Hitboxes->Update(Con);
	Physics->Update(Con, DeltaTime);
}

void kra::Game::StoreState()
{
	NetSaveBuffer Buff(StateBuffer);

	//Input->
	Physics->NetSave(Buff);
	//Hitboxes->
	//Entities->

}

void kra::Game::RestoreState()
{
}

Context kra::Game::MakeContext()
{
	Context Ret;
	Ret.Entities = &*Entities;
	Ret.PhysicsObjects = &*Physics;
	Ret.Hitboxes = &*Hitboxes;
	Ret.Inputs = &*Input;
	Ret.StateMachines = &*StateMachines;
	return Ret;
}
