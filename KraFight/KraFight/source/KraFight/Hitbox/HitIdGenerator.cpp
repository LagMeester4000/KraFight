#include "KraFight/Hitbox/HitIdGenerator.h"

using namespace kra;

kra::HitIdGenerator::HitIdGenerator()
{
}

HitId kra::HitIdGenerator::Generate()
{
	HitId Ret;
	Ret.Id = Counter;
	Counter += HitId::HitIdSize;
	return Ret;
}
