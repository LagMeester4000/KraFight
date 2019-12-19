#pragma once
#include "KraFight/Detail/Pointer.h"
#include "KraFight/Detail/Function.h"
#include "KraFight/TypeDef.h"
#include "KraFight/Network/INetSerialize.h"
#include "KraFight/Context.h"

namespace kra {
	class PhysicsObject;
	struct HitProperties;

	class Entity : public INetSerialize {
	public:
		// Called when the Entity is created
		// This function should be used to create objects in various systems and store the handles to those objects
		virtual void OnCreated(const Context& Con, Handle<Entity> Self);
		
		// Called when the Entity is destroyed
		// This function should be used to clean up handles (destroy the objects they are associated to) if needed
		virtual void OnDestroyed(const Context& Con, Handle<Entity> Self);

		// Virtual update function implemented by child classes
		virtual void Update(kfloat DeltaTime, const Context& Con, Handle<Entity> Self) {}

		// Function called when a hurtbox (owned by this entity) has been hit
		// This function is called after OnSuccessfulHit on the other entity
		virtual void OnGetHit(const HitProperties& Hit, Handle<Entity> Other, const Context& Con, Handle<Entity> Self) {}

		// Function called when a hitbox (owned by this entity) has hit a hurtbox
		// This function is called before the OnGetHit on the other entity
		virtual void OnSuccessfulHit(const HitProperties& Hit, Handle<Entity> Other, const Context& Con, Handle<Entity> Self) {}

		// Get the handle to the entity's main physics body
		Handle<PhysicsObject> GetPhysicsBody() const;

	public: // Networking
		// Function to save the object into the buffer
		void NetSave(NetSaveBuffer& Buff) override;

		// Function to load the object from the buffer
		void NetLoad(NetLoadBuffer& Buff) override;

	protected:
		// Handle to root physics object, every entity must have one
		Handle<PhysicsObject> PhysicsBody;
	};
}