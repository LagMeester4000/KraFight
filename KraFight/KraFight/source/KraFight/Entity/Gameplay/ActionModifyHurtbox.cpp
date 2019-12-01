#include "ActionModifyHurtbox.h"
#include "Essentials.h"
#include "PlayerCharacter.h"

kra::ActionModifyHurtbox::ActionModifyHurtbox()
{
}

void kra::ActionModifyHurtbox::Activate(const Context & Con, Handle<Entity> Ent)
{
	//auto Owner = Con.Entities->Get(Ent);
	//if (auto Player = PointerDynCast<PlayerCharacter>(Owner))
	//{
	//	// Quit if attack is invalid
	//	if (!Player->HurtboxBody.IsValid())
	//		return;
	//
	//	auto& Hurt = Con.Hitboxes->GetHurtbox(Player->HurtboxBody);
	//	Hurt.SetHurtbox(Index, NewHurtbox);
	//}
}
