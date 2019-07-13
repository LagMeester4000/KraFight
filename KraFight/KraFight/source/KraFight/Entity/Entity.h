#pragma once
#include "KraFight/Detail/Pointer.h"
#include "KraFight/Detail/Function.h"
#include "KraFight/TypeDef.h"
#include "KraFight/Network/INetSerialize.h"
#include "KraFight/Detail/Handle.h"

namespace kra {
	struct Context;
	class PhysicsObject;

	class Entity : public INetSerialize {
	public:
		// Called when the Entity is created
		// This function should be used to create objects in various systems and store the handles to those objects
		virtual void OnCreated(const Context& Con);
		
		// Called when the Entity is destroyed
		// This function should be used to clean up handles (destroy the objects they are associated to) if needed
		virtual void OnDestroyed(const Context& Con);

		// Virtual update function implemented by child classes
		virtual void Update(kfloat DeltaTime, const Context& Con) {}

	protected:
		// Handle to root physics object, every entity must have one
		Handle<PhysicsObject> PhysicsBody;
	};
}