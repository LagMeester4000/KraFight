#pragma once
#include "KraFight/Detail/Manager.h"
#include "KraFight/TypeDef.h"

namespace kra {
	class Entity;
	struct Context;

	class EntityManager {
	public:
		EntityManager();

		// Update the entities
		void Update(const Context& Con, kfloat DeltaTime);

		// Get an entity
		Pointer<Entity> Get(Handle<Entity> Hand);
		
		// Create a new entity
		Handle<Entity> Add(Pointer<Entity> NewEnt, const Context& Con);

		// Destroy an entity
		void Destroy(Handle<Entity> Hand, const Context& Con);

	private:
		Manager<Entity> Entities;
		Handle<Entity> Player1;
		Handle<Entity> Player2;
	};
}