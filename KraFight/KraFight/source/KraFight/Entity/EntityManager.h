#pragma once
#include "KraFight/Detail/Manager.h"

namespace kra {
	class EntityManager {
	public:

	private:
		Manager<Entity> Entities;
		Handle<Entity> Player1;
		Handle<Entity> Player2;
	};
}