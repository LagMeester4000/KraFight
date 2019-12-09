#pragma once
#include "KraFight/Math/Vector2.h"
#include "KraFight/Behavior/Attack.h"

namespace kra {
	struct Hitbox;

	struct AttackFuncs {
		static void SpawnHitbox(const AttackContext& Con, int Index, Hitbox& Hit);
		static void SpawnHitboxAir(const AttackContext& Con, int Index, Vector2 Position, Vector2 Size, kfloat Damage, kfloat Hitstun, kfloat LaunchAngle, kfloat Knockback);
		static void SpawnHitboxGround(const AttackContext& Con, int Index, Vector2 Position, Vector2 Size, kfloat Damage, kfloat Hitstun, kfloat Knockback);

		static void StopHitbox(const AttackContext& Con, int Index);
	};
}