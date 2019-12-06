#pragma once
#include "KraFight/Entity/Entity.h"
#include "KraFight/Detail/Handle.h"
#include "KraFight/Detail/AnyPtr.h"

namespace kra {
	// Hook interface
	// An instance of this class should be made for each entity type
	class IHook {
	public:
		virtual void SpawnEntity(const Context& Con, AnyPtr Hook, Handle<Entity> NewEnt);
		virtual void DestroyEntity(const Context& Con, AnyPtr Hook, Handle<Entity> Ent);
	};
}