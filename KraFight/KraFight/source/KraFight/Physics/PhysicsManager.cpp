#include "PhysicsManager.h"

kra::PhysicsManager::PhysicsManager()
{
}

void kra::PhysicsManager::Update(const Context& Con, kfloat DeltaTime)
{
	for (auto& It : Objects)
	{
		if (It.Exists)
		{
			It.Value.Update(DeltaTime);
			It.Value.UpdateCollision(Con, WallDistance);
		}
	}
}
