#pragma once
#include "KraFight/Math/FixedPoint.h"

namespace kra {
	// Floating point type that should be used everywhere in the project
	// Might be replaced with a custom floating point type later on
	using kfloat = FixedPoint<int32_t, 12>;

	// Custom postfix
	kfloat operator"" _k(uint64_t Num);
}