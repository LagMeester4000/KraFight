#include "HitboxCollection.h"

using namespace kra;

kra::HitboxCollection::HitboxCollection(int PlayerNum)
	: PlayerNumber(PlayerNum)
{
}

void kra::HitboxCollection::ClearHitboxes()
{
	for (auto& It : Hitboxes)
	{
		It.bActive = false;
	}
}

void kra::HitboxCollection::SetHitbox(size_t I, const Hitbox & Hit)
{
	if (Hitboxes.size() - 1 < I)
	{
		Hitboxes.resize(I + 2);
	}

	Hitboxes[I] = Hit;
}

const std::vector<Hitbox>& kra::HitboxCollection::GetContainer() const
{
	return Hitboxes;
}

Handle<Entity> kra::HitboxCollection::GetOwner() const
{
	return Owner;
}

int kra::HitboxCollection::GetPlayerNumber() const
{
	return PlayerNumber;
}
