#include "KraFight/Entity/EntityManager.h"
#include "KraFight/Entity/Entity.h"

using namespace kra;

kra::EntityManager::EntityManager()
{
}

void kra::EntityManager::Update(const Context & Con, kfloat DeltaTime)
{
	auto& Container = Entities.Container();
	for (size_t I = 0; I < Container.size(); ++I)
	{
		auto& It = Container[I];
		if (It.Exists)
		{
			Handle<Entity> Hand((HandleT)I);
			It.Value->Update(DeltaTime, Con, Hand);
		}
	}
}

Pointer<Entity> & kra::EntityManager::Get(Handle<Entity> Hand)
{
	return Entities.Get(Hand);
}

Handle<Entity> kra::EntityManager::Add(Pointer<Entity> NewEnt, const Context & Con)
{
	auto Hand = Entities.Add(NewEnt);
	NewEnt->OnCreated(Con, Hand);
	return Hand;
}

void kra::EntityManager::Destroy(Handle<Entity> Hand, const Context& Con)
{
	auto Point = Entities.Get(Hand);
	Point->OnDestroyed(Con, Hand);
	Entities.Destroy(Hand);
}
