#include "TimelineResource.h"
#include "TimelineAction.h"

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
