#pragma once
#include "KraFight/TypeDef.h"

namespace kra {
	// Struct containing all the base attribute values of a character
	// Examples: walking speed, jumping height/length, dash distance/speed
	struct PlayerAttributes {
		kfloat WalkSpeed = 100_k;
		kfloat DashSpeed, DashDuration;
		kfloat JumpVSpeed, JumpHSpeed, JumpGravity;
	};
}