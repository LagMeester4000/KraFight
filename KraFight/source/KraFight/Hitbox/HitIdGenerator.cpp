#include "KraFight/Hitbox/HitIdGenerator.h"
#include "KraFight/Network/NetLoadBuffer.h"
#include "KraFight/Network/NetSaveBuffer.h"

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

void kra::HitIdGenerator::NetSave(NetSaveBuffer & Buff)
{
	Buff << Counter;
}

void kra::HitIdGenerator::NetLoad(NetLoadBuffer & Buff)
{
	Buff >> Counter;
}
