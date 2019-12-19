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

void kra::StateMachineManager::NetSave(NetSaveBuffer & Buff)
{
	auto& C = Container();
	size_t Size = C.size();
	Buff << Size;

	for (size_t I = 0; I < Size; ++I)
	{
		auto& It = C[I];
		Buff << It.Exists;
		if (It.Exists)
		{
			It.Value->NetSave(Buff);
		}
	}
}

void kra::StateMachineManager::NetLoad(NetLoadBuffer & Buff)
{
	auto& C = Container();
	size_t Size;
	Buff >> Size;

	for (size_t I = 0; I < Size; ++I)
	{
		auto& It = C[I];
		Buff >> It.Exists;
		if (It.Exists)
		{
			It.Value->NetLoad(Buff);
		}
	}
}
