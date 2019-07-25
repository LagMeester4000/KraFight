#pragma once
#include "StateMachineDef.h"
#include "KraFight/Detail/Manager.h"

namespace kra {
	class Entity;

	class StateMachineManager : public Manager<PlayerStateMachine> {
	public:
		StateMachineManager();

	private:

	};
}