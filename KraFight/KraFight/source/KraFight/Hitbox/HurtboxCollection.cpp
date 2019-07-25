#include "HurtboxCollection.h"
#include "HitboxCollection.h"
#include "KraFight/Context.h"
#include "KraFight/Physics/PhysicsManager.h"
#include "KraFight/Entity/EntityManager.h"
#include "KraFight/Entity/Entity.h"

using namespace kra;

kra::HurtboxCollection::HurtboxCollection(int PlayerNum)
	: PlayerNumber(PlayerNum)
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

		for (auto& Hit : Hit.GetContainer())
		{
			if (!Hit.bActive)
			{
				continue;
			}

			if (Overlap(Hurt, MyPhysics.GetPosition(), Hit, OtherPhysics.GetPosition()))
			{
				Ret.bHit = true;
				if (Hit.HitProps.LocalPriority >= Ret.Properties.LocalPriority)
				{
					Ret.Properties = Hit.HitProps;
				}
			}	
		}
	}

	return Ret;
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
	if (Hurtboxes.size() - 1 < I)
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
