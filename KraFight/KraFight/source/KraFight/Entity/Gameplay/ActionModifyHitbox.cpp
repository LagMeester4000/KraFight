#include "ActionModifyHitbox.h"
#include "Essentials.h"
#include "PlayerCharacter.h"

using namespace kra;

kra::ActionModifyHitbox::ActionModifyHitbox()
{
}

void kra::ActionModifyHitbox::Activate(const Context & Con, Handle<Entity> Ent)
{
	//auto Owner = Con.Entities->Get(Ent);
	//if (auto Player = PointerDynCast<PlayerCharacter>(Owner))
	//{
	//	// Quit if attack is invalid
	//	if (!Player->HitboxHandle.IsValid())
	//		return;
	//
	//	auto& Hit = Con.Hitboxes->GetHitbox(Player->HitboxHandle);
	//	Hit.SetHitbox(Index, NewHitbox);
	//}
}