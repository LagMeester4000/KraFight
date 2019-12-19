#include "KraFight/Game.h"
#include "KraFight/Input/InputManager.h"
#include "KraFight/Physics/PhysicsManager.h"
#include "KraFight/Hitbox/HitboxManager.h"
#include "KraFight/Entity/EntityManager.h"
#include "KraFight/Behavior/StateMachineManager.h"
#include "KraFight/Resource/ResourceManager.h"
#include "KraFight/Network/NetSaveBuffer.h"
#include "KraFight/Network/NetLoadBuffer.h"
#include "KraFight/Entity/Gameplay/PlayerCharacter.h"

using namespace kra;

kra::Game::Game()
{
	Input = MakePointer<InputManager>();
	Physics = MakePointer<PhysicsManager>();
	Hitboxes = MakePointer<HitboxManager>();
	Entities = MakePointer<EntityManager>();
	StateMachines = MakePointer<StateMachineManager>();
	Resources = MakePointer<ResourceManager>();
}

void kra::Game::Update(kfloat DeltaTime, const InputFrame & P1Input, const InputFrame & P2Input)
{
	Input->Insert(Handle<InputBuffer>(0), P1Input);
	Input->Insert(Handle<InputBuffer>(1), P2Input);

	auto Con = MakeContext();

	StateMachines->Update(Con, DeltaTime);
	Entities->Update(Con, DeltaTime);
	Hitboxes->Update(Con);

	auto P1 = GetPlayer(0);
	auto P2 = GetPlayer(1);
	Physics->Update(Con, DeltaTime, P1->GetPhysicsBody(), P2->GetPhysicsBody());
}

void kra::Game::StoreState()
{
	NetSaveBuffer Buff(StateBuffer);

	Input->NetSave(Buff);
	Physics->NetSave(Buff);
	Hitboxes->NetSave(Buff);
	Entities->NetSave(Buff);
	StateMachines->NetSave(Buff);
}

void kra::Game::RestoreState()
{
	NetLoadBuffer Buff(StateBuffer);

	Input->NetLoad(Buff);
	Physics->NetLoad(Buff);
	Hitboxes->NetLoad(Buff);
	Entities->NetLoad(Buff);
	StateMachines->NetLoad(Buff);
}

Context kra::Game::MakeContext()
{
	Context Ret;
	Ret.Entities = &*Entities;
	Ret.PhysicsObjects = &*Physics;
	Ret.Hitboxes = &*Hitboxes;
	Ret.Inputs = &*Input;
	Ret.StateMachines = &*StateMachines;
	Ret.Resources = &*Resources;
	return Ret;
}

void kra::Game::SetupPlayers(Pointer<PlayerCharacter> Point, Pointer<PlayerCharacter> Point2)
{
	auto Con = MakeContext();

	auto SM1Point = kra::MakePointer<PlayerStateMachine>(EPlayerStates::Idle);
	auto SM1 = Con.StateMachines->Add(SM1Point);
	auto SM2Point = kra::MakePointer<PlayerStateMachine>(EPlayerStates::Idle);
	auto SM2 = Con.StateMachines->Add(SM2Point);

	auto P1 = Con.Entities->Add(Point, Con);
	auto P2 = Con.Entities->Add(Point2, Con);
	Point->SetupPlayer(kra::Handle<kra::InputBuffer>(0), SM1, 0, P2);
	Point2->SetupPlayer(kra::Handle<kra::InputBuffer>(1), SM2, 1, P1);

	SM1Point->SetOwner(P1);
	SM2Point->SetOwner(P2);

	auto SMS1 = kra::PlayerStateMachineSetup(*SM1Point);
	Point->SetupStateMachine(SMS1);
	auto SMS2 = kra::PlayerStateMachineSetup(*SM2Point);
	Point2->SetupStateMachine(SMS2);

	Player1 = P1;
	Player2 = P2;
}

Pointer<PlayerCharacter> kra::Game::GetPlayer(int PlayerIndex)
{
	if (PlayerIndex == 0)
	{
		return PointerDynCast<PlayerCharacter>(Entities->Get(Player1));
	}
	else if (PlayerIndex == 1)
	{
		return PointerDynCast<PlayerCharacter>(Entities->Get(Player2));
	}
	return nullptr;
}

Handle<Entity> kra::Game::GetPlayerHandle(int PlayerIndex)
{
	if (PlayerIndex == 0)
	{
		return Player1;
	}
	else if (PlayerIndex == 1)
	{
		return Player2;
	}
	return Handle<Entity>();
}