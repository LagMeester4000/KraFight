#pragma once
#include "KraFight/TypeDef.h"

namespace kra {
	enum class HitHeight {
		Middle,
		Low,
		Overhead
	};

	struct HitProperties {
		int Damage = 10;
		kfloat FreezeFrames = 5_k;
		kfloat BlockStun = 5_k;
		int HitNumber = 0;
		HitHeight Height = HitHeight::Middle;

		// Ground knockback
		kfloat GroundHitstun = 15_k;
		kfloat GroundKnockback = 100_k;
		kfloat GroundKnockbackOnBlock = 20_k;

		// Air knockback
		kfloat AirHitstun = 15.f;
		bool bLauncher = false;
		kfloat LaunchAngle = 70_k;
		kfloat LaunchSpeed = 100_k;

		// Priority
		int LocalPriority = 0;
		int GlobalPriority = 0;

		// Special
		bool bSpecialCancel = false;
		kfloat MeterOnHit = 10_k;
		kfloat MeterOnBlock = 5_k;
	};
}