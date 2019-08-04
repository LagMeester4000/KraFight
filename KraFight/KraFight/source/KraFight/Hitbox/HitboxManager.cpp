#include "HitboxManager.h"
#include "HitCollision.h"
#include "KraFight/Context.h"
#include "KraFight/Physics/PhysicsManager.h"
#include "KraFight/Entity/EntityManager.h"
#include "KraFight/Entity/Entity.h"
#include <set>

using namespace kra;

kra::HitboxManager::HitboxManager()
{
}

void kra::HitboxManager::Update(const Context & Con)
{
	struct HitRecord {
		HitProperties Props;
		Handle<Entity> GotHit;
		Handle<Entity> DidHit;

		// For handling repeated hits
		Handle<HurtboxCollection> Hurt;
		HitId Id;
	};
	std::vector<HitRecord> Hits;

	// Gather all the hits
	for (size_t HurtI = 0; HurtI < Hurtboxes.Container().size(); ++HurtI)
	{
		auto& Hurt = Hurtboxes.Container()[HurtI];
		if (!Hurt.Exists)
		{
			continue;
		}

		for (auto& Hit : Hitboxes.Container())
		{
			if (!Hit.Exists)
			{
				continue;
			}

			auto HitTest = Hurt.Value.Collide(Con, Hit.Value);
			if (HitTest.bHit)
			{
				HitRecord Rec;
				Rec.Props = HitTest.Properties;
				Rec.GotHit = Hurt.Value.GetOwner();
				Rec.DidHit = Hit.Value.GetOwner();
				Rec.Hurt = Handle<HurtboxCollection>((HandleT)HurtI);
				Rec.Id = Hit.Value.GetHitId();
				Hits.push_back(Rec);
			}
		}
	}

	// Check if the hits overlap, if so we need to check for the priority of the attacks
	std::set<HitRecord*> FinalHits;
	for (size_t BaseI = 0; BaseI < Hits.size(); ++BaseI)
	{
		bool bCanAdd = true;

		for (size_t I = 0; I < Hits.size(); ++I)
		{
			if (I == BaseI)
			{
				continue;
			}

			if (Hits[BaseI].GotHit == Hits[I].DidHit)
			{
				if (Hits[BaseI].Props.GlobalPriority < Hits[I].Props.GlobalPriority)
				{
					bCanAdd = false;
				}
			}
		}

		if (bCanAdd)
		{
			FinalHits.emplace(&Hits[BaseI]);
		}
	}

	// Actual final hit handling
	for (auto& It : FinalHits)
	{
		// Add hit to the list
		GetHurtbox(It->Hurt).RegisterHit(It->Id.Offset(It->Props.HitNumber));

		auto GotHit = Con.Entities->Get(It->GotHit);
		auto DidHit = Con.Entities->Get(It->DidHit);

		auto DidHitCon = Con.Make(It->DidHit);
		auto GotHitCon = Con.Make(It->GotHit);
		DidHit->OnSuccessfulHit(It->Props, It->GotHit, DidHitCon);
		GotHit->OnGetHit(It->Props, It->DidHit, GotHitCon);
	}
}

HitboxCollection & kra::HitboxManager::GetHitbox(Handle<HitboxCollection> Handl)
{
	return Hitboxes.Get(Handl);
}

Handle<HitboxCollection> kra::HitboxManager::AddHitbox(Handle<Entity> Owner, int PlayerNumber)
{
	auto Push = HitboxCollection(PlayerNumber, HitGenerator.Generate());
	return Hitboxes.Add(Push);
}

HurtboxCollection & kra::HitboxManager::GetHurtbox(Handle<HurtboxCollection> Handl)
{
	return Hurtboxes.Get(Handl);
}

Handle<HurtboxCollection> kra::HitboxManager::AddHurtbox(Handle<Entity> Owner, int PlayerNumber)
{
	auto Push = HurtboxCollection(PlayerNumber);
	return Hurtboxes.Add(Push);
}

void kra::HitboxManager::DestroyHitbox(Handle<HitboxCollection> Hand)
{
	Hitboxes.Destroy(Hand);
}

void kra::HitboxManager::DestroyHurtbox(Handle<HurtboxCollection> Hand)
{
	Hurtboxes.Destroy(Hand);
}
