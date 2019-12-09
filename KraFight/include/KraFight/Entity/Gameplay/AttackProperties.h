#pragma once
#include "AttackPosition.h"

namespace kra {
	struct AttackProperties {
		// Should have enum value
		int AttackType;

		// Where the attack is used
		AttackPosition Position;
	};
}