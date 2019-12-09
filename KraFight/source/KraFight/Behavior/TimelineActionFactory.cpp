#include "KraFight/Behavior/TimelineActionFactory.h"

using namespace kra;

kra::TimelineActionFactory::TimelineActionFactory()
{
}

Pointer<TimelineAction> kra::TimelineActionFactory::Create(const std::string & Name)
{
	auto& El = Elements[Name];
	return El.CreateFunction.func();
}

const std::map<std::string, TimelineActionFactoryElement>& kra::TimelineActionFactory::GetAllElements()
{
	return Elements;
}
