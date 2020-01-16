#include "KraFight/Entity/Gameplay/AttackFuncs.h"
#include "KraFight/Entity/Gameplay/PlayerCharacter.h"
#include "KraFight/Hitbox/HitboxManager.h"
#include "KraFight/Entity/EntityManager.h"

void kra::AttackFuncs::SpawnHitbox(const AttackContext & Con, int Index, Hitbox & Hit)
{
	auto& Ent = Con.Context.Entities->Get(Con.Entity);
	if (auto AsPlayer = PointerDynCast<PlayerCharacter>(Ent))
	{
		auto& Hitt = Con.Context.Hitboxes->GetHitbox(AsPlayer->GetHitbox());
		Hitt.SetHitbox(Index, Hit);
	}
}

void kra::AttackFuncs::SpawnHitboxAir(const AttackContext & Con, int Index, Vector2 Position, Vector2 Size, kfloat Damage, kfloat Hitstun, kfloat LaunchAngle, kfloat Knockback)
{
	auto& Ent = Con.Context.Entities->Get(Con.Entity);
	if (auto AsPlayer = PointerDynCast<PlayerCharacter>(Ent))
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
	auto& Ent = Con.Context.Entities->Get(Con.Entity);
	if (auto AsPlayer = PointerDynCast<PlayerCharacter>(Ent))
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
	auto& Ent = Con.Context.Entities->Get(Con.Entity);
	if (auto AsPlayer = PointerDynCast<PlayerCharacter>(Ent))
	{
		auto& Hit = Con.Context.Hitboxes->GetHitbox(AsPlayer->GetHitbox());
		
		Hitbox Box;
		Box.bActive = false;

		Hit.SetHitbox(Index, Box);
	}
}
