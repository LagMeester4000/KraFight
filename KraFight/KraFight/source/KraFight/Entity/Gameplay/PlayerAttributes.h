#pragma once
#include "KraFight/TypeDef.h"

namespace kra {
	// Struct containing all the base attribute values of a character
	// Examples: walking speed, jumping height/length, dash distance/speed
	struct PlayerAttributes {
		kfloat WalkSpeed = 150_k;
		kfloat DashSpeed, DashDuration;
		kfloat JumpVSpeed = 500_k, 
			JumpHSpeed = 150_k, 
			JumpGravity;
	};
}