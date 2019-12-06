#include "KraFight/Behavior/StateMachineManager.h"

kra::StateMachineManager::StateMachineManager()
{
}

void kra::StateMachineManager::Update(const Context & Con, kfloat DeltaTime)
{
	for (auto& It : Container())
	{
		if (It.Exists)
		{
			It.Value->Update(Con, DeltaTime);
		}
	}
}
