#include "HitId.h"

using namespace kra;

kra::HitId::HitId()
{
}

HitId kra::HitId::Offset(HitIdT Off) const
{
	HitId Ret;
	Ret.Id = Id + Off;
	return Ret;
}

HitIdT kra::HitId::Value() const
{
	return Id;
}

bool kra::HitId::operator==(const HitId & Other) const
{
	return Id == Other.Id;
}

bool kra::HitId::operator!=(const HitId & Other) const
{
	return Id != Other.Id;
}
