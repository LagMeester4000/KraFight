#include "KraFight/Hook/HookEvents.h"

void kra::HookEvents::Clear()
{
	OnCreateEntity.clear();
	OnDestroyEntity.clear();
}
