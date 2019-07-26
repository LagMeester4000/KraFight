#include "HitIdChecklist.h"

using namespace kra;

kra::HitIdChecklist::HitIdChecklist()
{
}

void kra::HitIdChecklist::Push(HitId NewHit)
{
	for (size_t I = Checklist.size() - 1; I > 0; --I)
	{
		Checklist[I] = Checklist[I - 1];
	}
	Checklist[0] = NewHit;
}

bool kra::HitIdChecklist::Check(HitId Hit)
{
	for (auto& It : Checklist)
	{
		if (It == Hit)
		{
			return true;
		}
	}
	return false;
}
