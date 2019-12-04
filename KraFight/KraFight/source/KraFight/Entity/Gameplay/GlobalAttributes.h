#pragma once
#include "KraFight/TypeDef.h"

namespace kra {
	// Struct with values that affect every character the same way
	struct GlobalAttributes {
		kfloat Gravity = 5_k;
		kfloat HitstunGroundFriction = 500_k;
		kfloat NormalGroundFriction = 500_k;

		const static GlobalAttributes Instance;
	};
}