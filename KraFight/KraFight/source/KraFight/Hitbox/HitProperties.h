#pragma once
#include "KraFight/TypeDef.h"

namespace kra {
	struct HitProperties {
		int Damage = 10;
		kfloat FreezeFrames = 5.f;
		kfloat BlockStun = 5.f;

		// Ground knockback
		kfloat GroundHitstun = 15.f;
		kfloat GroundKnockback = 100.f;
		kfloat GroundKnockbackOnBlock = 20.f;

		// Air knockback
		kfloat AirHitstun = 15.f;
		bool bLauncher = false;
		kfloat LaunchAngle = 70.f;
		kfloat LaunchSpeed = 100.f;

		// Priority
		int LocalPriority = 0;
		int GlobalPriority = 0;

		// Special
		bool bSpecialCancel = false;
		kfloat MeterOnHit = 10.f;
		kfloat MeterOnBlock = 5.f;
	};
}