#pragma once
#include "StateMachine.h"
#include "KraFight/Detail/Manager.h"
#include "KraFight/Entity/Gameplay/PlayerStates.h"

namespace kra {
	class Entity;

	class StateMachineManager : public Manager<StateMachine<Entity, EPlayerStates, EPlayerStates::MAX>> {
	public:
		StateMachineManager();

	private:

	};
}