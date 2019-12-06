#include "KraFight/Physics/PhysicsObject.h"

using namespace kra;

kra::PhysicsObject::PhysicsObject(Handle<Entity> Ent)
	: Owner(Ent), GravityForce(0)
{
}

kra::PhysicsObject::PhysicsObject(Handle<Entity> Ent, Vector2 Position, Vector2 Size)
	: Owner(Ent), Position(Position), Size(Size), GravityForce(0)
{
}

void kra::PhysicsObject::Update(kfloat DeltaTime)
{
	if (bFrozen)
		return;

	Velocity += Vector2{ 0_k, -GravityForce };
	Position += Velocity * DeltaTime;
}

void kra::PhysicsObject::UpdateCollision(const Context & Con, kfloat WallDistance)
{
	kfloat LowestY = Position.Y - Size.Y / 2_k;
	kfloat LowestX = Position.X - Size.X / 2_k;
	kfloat BiggestX = Position.X + Size.X / 2_k;
	
	// Test for ground hit
	if (LowestY < 0_k)
	{
		// Ground has been hit!
		kfloat NewYPos = /* 0.f + */ Size.Y / 2_k;
		Position.Y = NewYPos;

		// Update OnGround variable
		bOnGround = true;
	}

	// Test for left wall hit
	if (LowestX < -WallDistance)
	{
		// Left wall has been hit!
		Position.X = -WallDistance + Size.X / 2_k;
	}
	// Test for right wall hit
	else if (BiggestX > WallDistance)
	{
		// Right wall has been hit!
		Position.X = WallDistance - Size.X / 2_k;
	}

	// Check if physics object is leaving the ground
	if (Velocity.Y > 0_k)
	{
		bOnGround = false;
	}
}

Vector2 kra::PhysicsObject::GetPosition() const
{
	return Position;
}

void kra::PhysicsObject::SetPosition(const Vector2 & NewPosition)
{
	Position = NewPosition;
}

Vector2 kra::PhysicsObject::GetSize() const
{
	return Size;
}

void kra::PhysicsObject::SetSize(const Vector2 & NewSize)
{
	Size = NewSize;
}

Vector2 kra::PhysicsObject::GetVelocity() const
{
	return Velocity;
}

void kra::PhysicsObject::SetVelocity(const Vector2 & NewVelocity)
{
	Velocity = NewVelocity;
}

kfloat kra::PhysicsObject::GetGravityForce() const
{
	return GravityForce;
}

void kra::PhysicsObject::SetGravityForce(kfloat NewGravity)
{
	GravityForce = NewGravity;
}

bool kra::PhysicsObject::IsFrozen() const
{
	return bFrozen;
}

void kra::PhysicsObject::SetIsFrozen(bool IsFrozen)
{
	bFrozen = IsFrozen;
}

bool kra::PhysicsObject::IsOnGround() const
{
	return bOnGround;
}

void kra::PhysicsObject::ForceToGround(bool ResetYVelocity)
{
	Position.Y = Size.Y / 2_k;
	bOnGround = true;

	if (ResetYVelocity)
	{
		Velocity.Y = 0_k;
	}
}

Handle<Entity> kra::PhysicsObject::GetOwner() const
{
	return Owner;
}

void kra::PhysicsObject::SetOwner(Handle<Entity> Ent)
{
	Owner = Ent;
}

bool kra::PhysicsObject::TestCollision(const PhysicsObject & Other)
{
	return	(Position.X - Size.X / 2_k < Other.Position.X + Other.Size.X / 2_k) &&
			(Position.X + Size.X / 2_k > Other.Position.X - Other.Size.X / 2_k) &&
			(Position.Y - Size.Y / 2_k < Other.Position.Y + Other.Size.Y / 2_k) &&
			(Position.Y + Size.Y / 2_k > Other.Position.Y - Other.Size.Y / 2_k);
}
