#include "Game.h"
#include "Input/InputManager.h"
#include "Physics/PhysicsManager.h"
#include "Hitbox/HitboxManager.h"
#include "Entity/EntityManager.h"
#include "Network/NetSaveBuffer.h"
#include "Network/NetLoadBuffer.h"

using namespace kra;

kra::Game::Game()
{
}

void kra::Game::Update(kfloat DeltaTime, const InputFrame & P1Input, const InputFrame & P2Input)
{
	Input->
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
	return Ret;
}
