#include "KraFight/TypeDef.h"

using namespace kra;

kfloat kra::operator""_k(uint64_t Num)
{
	return kfloat::makeFromInt(Num);
}
