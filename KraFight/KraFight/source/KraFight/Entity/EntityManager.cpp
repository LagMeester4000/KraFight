#include "EntityManager.h"
#include "Entity.h"

using namespace kra;

kra::EntityManager::EntityManager()
{
}

void kra::EntityManager::Update(const Context & Con, kfloat DeltaTime)
{
	for (auto& It : Entities.Container())
	{
		if (It.Exists)
		{
			It.Value->Update(DeltaTime, Con);
		}
	}
}

Pointer<Entity> kra::EntityManager::Get(Handle<Entity> Hand)
{
	return Entities.Get(Hand);
}

Handle<Entity> kra::EntityManager::Add(Pointer<Entity> NewEnt, const Context & Con)
{
	auto Hand = Entities.Add(NewEnt);
	NewEnt->OnCreated(Con);
	return Hand;
}

void kra::EntityManager::Destroy(Handle<Entity> Hand, const Context& Con)
{
	auto Point = Entities.Get(Hand);
	Point->OnDestroyed(Con);
	Entities.Destroy(Hand);
}
