#pragma once
#include "KraFight/Detail/Handle.h"

namespace kra {
	class Entity;
	struct Context;

	class TimelineAction {
	public:
		virtual ~TimelineAction() {}

		// Function called when the timeline action is activated
		virtual void Activate(const Context& Con, Handle<Entity> Ent) = 0;
	};
}