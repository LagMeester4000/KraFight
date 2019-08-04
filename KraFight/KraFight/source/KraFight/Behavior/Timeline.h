#pragma once
#include "KraFight/TypeDef.h"
#include "KraFight/Detail/Handle.h"

namespace kra {
	class TimelineResource;
	struct Context;
	class Entity;

	class Timeline {
	public:
		Timeline();
		Timeline(TimelineResource* Time);

		// Update the timeline
		void Update(kfloat DeltaTime, const Context& Con, Handle<Entity> Ent);

		// Set pointer to resource
		void SetTimelineResource(TimelineResource* Resource);

	private:
		kfloat Time = 0_k;
		TimelineResource* TimelineRes;
	};
}