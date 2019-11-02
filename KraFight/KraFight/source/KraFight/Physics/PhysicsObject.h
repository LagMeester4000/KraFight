#pragma once
#include "KraFight/Math/Vector2.h"
#include "KraFight/Detail/Handle.h"

namespace kra {
	struct Context;
	class Entity;

	// Physics object that has the shape of an AABB (box)
	class PhysicsObject {
	public:
		PhysicsObject(Handle<Entity> Ent);
		PhysicsObject(Handle<Entity> Ent, Vector2 Position, Vector2 Size);

		// Update the position and velocity of the physics object
		// Does not do any collision checks
		void Update(kfloat DeltaTime);

		// Check if the physics object has collided with any walls or floors
		// If it does, perform a callback
		void UpdateCollision(const Context& Con, kfloat WallDistance);

		Vector2 GetPosition() const;
		void SetPosition(const Vector2& NewPosition);

		Vector2 GetSize() const;
		void SetSize(const Vector2& NewSize);

		Vector2 GetVelocity() const;
		void SetVelocity(const Vector2& NewVelocity);

		kfloat GetGravityForce() const;
		void SetGravityForce(kfloat NewGravity);

		bool IsFrozen() const;
		void SetIsFrozen(bool IsFrozen);

		// Returns if the physics object is on the ground
		bool IsOnGround() const;

		// Force the object to be grounded
		// To be used at the start of a match on the player characters
		void ForceToGround(bool ResetYVelocity = true);

		Handle<Entity> GetOwner() const;
		void SetOwner(Handle<Entity> Ent);

		// Check for collision between this and another physics object
		bool TestCollision(const PhysicsObject& Other);
	private:
		Vector2 Position;
		Vector2 Size;
		Vector2 Velocity;
		bool bOnGround = false;
		kfloat GravityForce;
		bool bFrozen = false;

		// Handle to owning entity, used for callbacks
		Handle<Entity> Owner;
	};
}