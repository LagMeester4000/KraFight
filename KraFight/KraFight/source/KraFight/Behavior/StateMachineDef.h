#pragma once
#include "StateMachine.h"
#include "KraFight/Entity/Gameplay/PlayerStates.h"
#include "KraFight/Detail/Pointer.h"

namespace kra {
	class Entity;
	using PlayerStateMachine = StateMachine<Entity, EPlayerStates, EPlayerStates::MAX>;
	using PlayerStateMachineSetup = typename PlayerStateMachine::Setup;
}

#define KRA_ACTIVE(Type, Player, Cont, Scope) [](const kra::Context& Cont, kra::Handle<kra::Entity> Ent) { Type Player = kra::PointerDynCast<Type>(Cont.Entities->Get(Ent)); if (Player) { Scope } }
#define KRA_CONDITION(Type, Player, Cont, Scope) [](const kra::Context& Cont, kra::Handle<kra::Entity> Ent) -> bool { Type Player = kra::PointerDynCast<Type>(Cont.Entities->Get(Ent)); if (Player) { Scope } return false; }
#define KRA_UPDATE(Type, Player, DT, Cont, Scope) [](const kra::Context& Cont, kra::kfloat DT, kra::Handle<kra::Entity> Ent) { Type Player = kra::PointerDynCast<Type>(Cont.Entities->Get(Ent)); if (Player) { Scope } }
