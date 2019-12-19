#include "KraFight/Hitbox/HitboxCollection.h"
#include "KraFight/Network/NetLoadBuffer.h"
#include "KraFight/Network/NetSaveBuffer.h"
#include "KraFight/Network/SerFuncs/Vector.h"

using namespace kra;

kra::HitboxCollection::HitboxCollection(Handle<Entity> Own, int PlayerNum, HitId Hit)
	: Owner(Own), PlayerNumber(PlayerNum), Id(Hit)
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
	if (Hitboxes.size() <= I)
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

HitId kra::HitboxCollection::GetHitId() const
{
	return Id;
}

void kra::HitboxCollection::NetSave(NetSaveBuffer & Buff)
{
	Buff << Hitboxes
		<< Owner
		<< PlayerNumber
		<< Id;
}

void kra::HitboxCollection::NetLoad(NetLoadBuffer & Buff)
{
	Buff >> Hitboxes
		>> Owner
		>> PlayerNumber
		>> Id;
}
