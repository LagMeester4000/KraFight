#include "HurtboxCollection.h"
#include "HitboxCollection.h"
#include "KraFight/Context.h"
#include "KraFight/Physics/PhysicsManager.h"
#include "KraFight/Entity/EntityManager.h"
#include "KraFight/Entity/Entity.h"

using namespace kra;

kra::HurtboxCollection::HurtboxCollection(Handle<Entity> Own, int PlayerNum)
	: Owner(Own), PlayerNumber(PlayerNum)
{
}

HitCollision kra::HurtboxCollection::Collide(const Context & Con, const HitboxCollection & Hit)
{
	HitCollision Ret;

	if (Hit.GetPlayerNumber() == GetPlayerNumber())
	{
		return Ret;
	}

	auto& MyPhysics = Con.PhysicsObjects->Get(Con.Entities->Get(GetOwner())->GetPhysicsBody());
	auto& OtherPhysics = Con.PhysicsObjects->Get(Con.Entities->Get(Hit.GetOwner())->GetPhysicsBody());
	
	for (auto& Hurt : Hurtboxes)
	{
		if (!Hurt.bActive)
		{
			continue;
		}

		for (auto& Hitt : Hit.GetContainer())
		{
			if (!Hitt.bActive || Hits.Check(Hit.GetHitId().Offset(Hitt.HitProps.HitNumber)))
			{
				continue;
			}

			if (Overlap(Hurt, MyPhysics.GetPosition(), Hitt, OtherPhysics.GetPosition()))
			{
				Ret.bHit = true;
				if (Hitt.HitProps.LocalPriority >= Ret.Properties.LocalPriority)
				{
					Ret.Properties = Hitt.HitProps;
				}
			}	
		}
	}

	return Ret;
}

void kra::HurtboxCollection::RegisterHit(HitId Hit)
{
	Hits.Push(Hit);
}

void kra::HurtboxCollection::ClearHurtboxes()
{
	for (auto& It : Hurtboxes)
	{
		It.bActive = false;
	}
}

void kra::HurtboxCollection::SetHurtbox(size_t I, const Hurtbox & Hit)
{
	if (Hurtboxes.size() <= I)
	{
		Hurtboxes.resize(I + 2);
	}

	Hurtboxes[I] = Hit;
}

const std::vector<Hurtbox>& kra::HurtboxCollection::GetContainer() const
{
	return Hurtboxes;
}

Handle<Entity> kra::HurtboxCollection::GetOwner() const
{
	return Owner;
}

int kra::HurtboxCollection::GetPlayerNumber() const
{
	return PlayerNumber;
}
