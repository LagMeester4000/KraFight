#pragma once
#include "KraFight/TypeDef.h"
#include "KraFight/Detail/Pointer.h"
#include "KraFight/Detail/Handle.h"
#include <vector>

namespace kra {
	class TimelineAction;
	struct Context;
	class Entity;

	struct TimelineElement {
		kfloat TimePoint;
		Pointer<TimelineAction> Action;
	};

	class TimelineResource {
	public:
		TimelineResource();

		// Run actions in the timeline
		size_t Run(kfloat TimePrevFrame, kfloat Time, const Context& Con, Handle<Entity> Owner, size_t PrevIndex = 0);

	private:
		std::vector<TimelineElement> Elements;
	};
}