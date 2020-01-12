#pragma once
#include "KraFight/Detail/Handle.h"
#include "KraFight/Detail/AnyPtr.h"

namespace kra {
	struct Context;
	class Entity;

	// Virtual base class for implementing events outside of the framework (sound, graphics)
	class IHook {
	public:
		virtual ~IHook() {}

		virtual void OnEntityCreate(const Context& Con, Handle<Entity> Ent) = 0;
		virtual void OnEntityDestroy(const Context& Con, Handle<Entity> Ent) = 0;
		virtual void OnCustomEvent(const Context& Con, AnyPtr& Event) = 0;
	};
}