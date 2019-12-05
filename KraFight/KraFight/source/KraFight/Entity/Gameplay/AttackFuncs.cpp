#include "AttackFuncs.h"
#include "PlayerCharacter.h"
#include "KraFight/Hitbox/HitboxManager.h"

void kra::AttackFuncs::SpawnHitbox(const AttackContext & Con, int Index, Hitbox & Hit)
{
	if (auto AsPlayer = dynamic_cast<PlayerCharacter*>(Con.Entity))
	{
		auto& Hitt = Con.Context.Hitboxes->GetHitbox(AsPlayer->HitboxHandle);
		Hitt.SetHitbox(Index, Hit);
	}
}

void kra::AttackFuncs::SpawnHitboxAir(const AttackContext & Con, int Index, Vector2 Position, Vector2 Size, kfloat Damage, kfloat Hitstun, kfloat LaunchAngle, kfloat Knockback)
{
	if (auto AsPlayer = dynamic_cast<PlayerCharacter*>(Con.Entity))
	{
		auto& Hit = Con.Context.Hitboxes->GetHitbox(AsPlayer->GetHitbox());

		Hitbox Box;
		Box.bActive = true;
		Box.Position = Position;
		Box.Position.X *= AsPlayer->GetFacingDirection();
		Box.Size = Size;
		Box.HitProps.Damage = Damage;
		Box.HitProps.AirHitstun = Hitstun;
		Box.HitProps.bLauncher = true;
		Box.HitProps.LaunchAngle = LaunchAngle;
		Box.HitProps.LaunchSpeed = Knockback;

		Hit.SetHitbox(Index, Box);
	}
}

void kra::AttackFuncs::SpawnHitboxGround(const AttackContext & Con, int Index, Vector2 Position, Vector2 Size, kfloat Damage, kfloat Hitstun, kfloat Knockback)
{
	if (auto AsPlayer = dynamic_cast<PlayerCharacter*>(Con.Entity))
	{
		auto& Hit = Con.Context.Hitboxes->GetHitbox(AsPlayer->GetHitbox());

		Hitbox Box;
		Box.bActive = true;
		Box.Position = Position;
		Box.Position.X *= AsPlayer->GetFacingDirection();
		Box.Size = Size;
		Box.HitProps.Damage = Damage;
		Box.HitProps.GroundHitstun = Hitstun;
		Box.HitProps.bLauncher = false;
		Box.HitProps.GroundKnockback = Knockback;

		Hit.SetHitbox(Index, Box);
	}
}

void kra::AttackFuncs::StopHitbox(const AttackContext & Con, int Index)
{
	if (auto AsPlayer = dynamic_cast<PlayerCharacter*>(Con.Entity))
	{
		auto& Hit = Con.Context.Hitboxes->GetHitbox(AsPlayer->GetHitbox());
		
		Hitbox Box;
		Box.bActive = false;

		Hit.SetHitbox(Index, Box);
	}
}
