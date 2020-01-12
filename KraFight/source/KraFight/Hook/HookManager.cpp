#include "KraFight/Hook/HookManager.h"
#include "KraFight/Hook/IHook.h"
#include <utility>

using namespace kra;

kra::HookManager::HookManager()
{
}

void kra::HookManager::Update(const Context & Con)
{
	if (!Hook)
	{
		// Run Entity events
		for (auto& It : Events.OnCreateEntity)
		{
			Hook->OnEntityCreate(Con, It.Ent);
		}
		for (auto& It : Events.OnDestroyEntity)
		{
			Hook->OnEntityDestroy(Con, It.Ent);
		}

		// Run custom events
		for (size_t I = 0; I < CustomEvents.Size(); ++I)
		{
			Hook->OnCustomEvent(Con, CustomEvents[I]);
		}
	}

	Events.Clear();
	CustomEvents.Reset();
}

void kra::HookManager::RegisterCreateEntity(Handle<Entity> Ent)
{
	Events.OnCreateEntity.push_back({ Ent });
}

void kra::HookManager::RegisterDestroyEntity(Handle<Entity> Ent)
{
	for (size_t I = 0; I < Events.OnCreateEntity.size(); ++I)
	{
		auto& It = Events.OnCreateEntity[I];

		if (It.Ent == Ent)
		{
			Events.OnCreateEntity.erase(Events.OnCreateEntity.begin() + I);
			// There won't be any more events with this ID
			return;
		}
	}

	Events.OnDestroyEntity.push_back({ Ent });
}

AnyPtr kra::HookManager::GetExternal() const
{
	return External;
}

const std::unique_ptr<IHook>& kra::HookManager::GetHook() const
{
	return Hook;
}

void kra::HookManager::SetExternal(AnyPtr Ext)
{
	External = Ext;
}

void kra::HookManager::SetHook(std::unique_ptr<IHook>&& HHook)
{
	Hook = std::move(HHook);
}
