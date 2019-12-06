#include "KraFight/Behavior/TimelineResource.h"
#include "KraFight/Behavior/TimelineAction.h"

using namespace kra;

kra::TimelineResource::TimelineResource()
{
}

size_t kra::TimelineResource::Run(kfloat TimePrevFrame, kfloat Time, const Context & Con, Handle<Entity> Owner, size_t PrevIndex)
{
	for (size_t I = PrevIndex; I < Elements.size(); ++I)
	{
		auto& It = Elements[I];
		if (It.TimePoint > Time)
		{
			return I;
		}

		if (It.TimePoint > TimePrevFrame)
		{
			It.Action->Activate(Con, Owner);
		}
	}
}

std::vector<TimelineElement>& kra::TimelineResource::Container()
{
	return Elements;
}
