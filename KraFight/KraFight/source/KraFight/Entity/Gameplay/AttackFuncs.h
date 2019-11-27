#pragma once
#include "KraFight/Math/Vector2.h"
#include "KraFight/Behavior/Attack.h"

namespace kra {
	struct AttackFuncs {
		static void SpawnHitboxAir(const AttackContext& Con, int Index, Vector2 Position, Vector2 Size, kfloat Damage, kfloat Hitstun, kfloat LaunchAngle, kfloat Knockback);
		static void SpawnHitboxGround(const AttackContext& Con, int Index, Vector2 Position, Vector2 Size, kfloat Damage, kfloat Hitstun, kfloat Knockback);

		static void StopHitbox(const AttackContext& Con, int Index);
	};
}