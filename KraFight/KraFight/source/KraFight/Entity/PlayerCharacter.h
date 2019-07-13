#pragma once
#include "Entity.h"

namespace kra {
	class PlayerCharacter : public Entity {
	public:
		virtual void SetupStateMachine();
	};
}