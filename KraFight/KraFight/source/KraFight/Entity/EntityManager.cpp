#include "EntityManager.h"
#include "Entity.h"

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
			auto NewCon = Con.Make(Hand);
			It.Value->Update(DeltaTime, NewCon);
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
	auto NewCon = Con.Make(Hand);
	NewEnt->OnCreated(NewCon);
	return Hand;
}

void kra::EntityManager::Destroy(Handle<Entity> Hand, const Context& Con)
{
	auto Point = Entities.Get(Hand);
	auto NewCon = Con.Make(Hand);
	Point->OnDestroyed(NewCon);
	Entities.Destroy(Hand);
}
