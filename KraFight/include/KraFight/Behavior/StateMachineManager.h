#pragma once
#include "StateMachineDef.h"
#include "KraFight/Detail/Manager.h"

namespace kra {
	class Entity;
	struct Context;

	class StateMachineManager : public Manager<PlayerStateMachine> {
	public:
		StateMachineManager();
		
		// Update all the state machines
		void Update(const Context& Con, kfloat DeltaTime);
	};
}