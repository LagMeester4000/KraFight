#pragma once
#include "StateMachineDef.h"
#include "KraFight/Detail/Manager.h"

namespace kra {
	class Entity;
	struct Context;
	class NetSaveBuffer;
	class NetLoadBuffer;

	class StateMachineManager : public Manager<PlayerStateMachine> {
	public:
		StateMachineManager();
		
		// Update all the state machines
		void Update(const Context& Con, kfloat DeltaTime);

	public: // Networking
		void NetSave(NetSaveBuffer& Buff);
		void NetLoad(NetLoadBuffer& Buff);
	};
}