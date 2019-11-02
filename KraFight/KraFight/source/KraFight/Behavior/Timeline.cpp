#include "Timeline.h"
#include "TimelineResource.h"

using namespace kra;

kra::Timeline::Timeline()
	: TimelineRes(nullptr)
{
}

kra::Timeline::Timeline(TimelineResource * Time)
	: TimelineRes(Time)
{
}

void kra::Timeline::Update(kfloat DeltaTime, const Context & Con, Handle<Entity> Ent)
{
	if (!bActive)
		return;

	auto NewTime = Time + DeltaTime;
	TimelineRes->Run(Time, NewTime, Con, Ent);
	Time = NewTime;
}

void kra::Timeline::SetTimelineResource(TimelineResource * Resource)
{
	TimelineRes = Resource;
}

void kra::Timeline::SetActive(bool Active)
{
	bActive = Active;
}

void kra::Timeline::Reset()
{
	Time = 0_k;
}
