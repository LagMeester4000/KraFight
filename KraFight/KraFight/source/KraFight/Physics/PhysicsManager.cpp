#include "PhysicsManager.h"

kra::PhysicsManager::PhysicsManager()
{
}

void kra::PhysicsManager::Update(const Context& Con, kfloat DeltaTime, Handle<PhysicsObject> Player1, Handle<PhysicsObject> Player2)
{
	/*UpdatePlayers(Player1, Player2);

	for (size_t I = 0; I < Objects.size(); ++I)
	{
		auto& It = Objects[I];
		if (It.Exists && I != Player1.GetHandle() && I != Player2.GetHandle())
		{
			It.Value.Update(DeltaTime);
		}
	}

	for (auto& It : Objects)
	{
		if (It.Exists)
		{
			It.Value.UpdateCollision(Con, WallDistance);
		}
	}*/

	for (auto& It : Objects)
	{
		if (It.Exists)
		{
			It.Value.Update(DeltaTime);
			It.Value.UpdateCollision(Con, WallDistance);
		}
	}

	UpdatePlayers(Con, Player1, Player2);
}

void kra::PhysicsManager::UpdatePlayers(const Context& Con, Handle<PhysicsObject> Player1, Handle<PhysicsObject> Player2)
{
	auto& P1Phys = Get(Player1);
	auto& P2Phys = Get(Player2);
	auto P1Vel = P1Phys.GetVelocity();
	auto P2Vel = P2Phys.GetVelocity();
	auto P1Pos = P1Phys.GetPosition();
	auto P2Pos = P2Phys.GetPosition();

	// Test collision
	if (!P1Phys.TestCollision(P2Phys))
		return;

	//bool OpposingForce = 
	//	(P1Pos.X < P2Pos.X && P1Vel.X > 0_k && P2Vel.X < 0_k) || 
	//	(P1Pos.X > P2Pos.X && P1Vel.X < 0_k && P2Vel.X > 0_k);
	//
	//bool SameState = P1Phys.IsOnGround() == P2Phys.IsOnGround();

	if (P1Pos.X < P2Pos.X)
	{
		// Calculate difference
		kfloat XDiff = (P1Pos.X + P1Phys.GetSize().X / 2_k) - (P2Pos.X - P2Phys.GetSize().X / 2_k);
		Vector2 Diff{ XDiff / 2_k, 0_k };

		// Set pos
		P1Phys.SetPosition(P1Pos - Diff);
		P2Phys.SetPosition(P2Pos + Diff);
	}
	else
	{
		// Calculate difference
		kfloat XDiff = (P2Pos.X + P2Phys.GetSize().X / 2_k) - (P1Pos.X - P1Phys.GetSize().X / 2_k);
		Vector2 Diff{ XDiff / 2_k, 0_k };

		// Set pos
		P1Phys.SetPosition(P1Pos + Diff);
		P2Phys.SetPosition(P2Pos - Diff);
	}

	// Update collision with walls and floor
	P1Phys.UpdateCollision(Con, WallDistance);
	P2Phys.UpdateCollision(Con, WallDistance);
}