#include "PhysicsObject.h"

using namespace kra;

kra::PhysicsObject::PhysicsObject()
{
	Owner.MakeInvalid();
}

kra::PhysicsObject::PhysicsObject(Vector2 Position, Vector2 Size)
	: Position(Position), Size(Size)
{
}

void kra::PhysicsObject::Update(kfloat DeltaTime)
{
	if (bFrozen)
		return;

	Velocity += Vector2{ 0.f, -GravityForce };
	Position += Velocity * DeltaTime;
}

void kra::PhysicsObject::UpdateCollision(const Context & Con, kfloat WallDistance)
{
	kfloat LowestY = Position.Y - Size.Y / kfloat(2.f);
	kfloat LowestX = Position.X - Size.X / kfloat(2.f);
	kfloat BiggestX = Position.X + Size.X / kfloat(2.f);
	
	// Test for ground hit
	if (LowestY < kfloat(0.f))
	{
		// Ground has been hit!
		kfloat NewYPos = /* 0.f + */ Size.Y / kfloat(2.f);
		Position.Y = NewYPos;
	}

	// Test for left wall hit
	if (LowestX < -WallDistance)
	{
		// Left wall has been hit!
		Position.X = -WallDistance + Size.X / kfloat(2.f);
	}
	// Test for right wall hit
	else if (BiggestX > WallDistance)
	{
		// Right wall has been hit!
		Position.X = WallDistance - Size.X / kfloat(2.f);
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

bool kra::PhysicsObject::TestCollision(const PhysicsObject & Other)
{
	return	(Position.X - Size.X / kfloat(2.f) > Other.Position.X + Other.Size.X * kfloat(2.f)) &&
			(Position.X + Size.X / kfloat(2.f) < Other.Position.X - Other.Size.X * kfloat(2.f)) && 
			(Position.Y - Size.Y / kfloat(2.f) > Other.Position.Y + Other.Size.Y * kfloat(2.f)) &&
			(Position.Y + Size.Y / kfloat(2.f) < Other.Position.Y - Other.Size.Y * kfloat(2.f));
}
