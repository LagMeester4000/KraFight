#pragma once
#include "KraFight/Entity/Entity.h"
#include "KraFight/Detail/Handle.h"

namespace kra {
	// Hook interface
	class IHook {
	public:
		virtual void SpawnEntity(const Context& Con, Handle<Entity> NewEnt);
	};
}