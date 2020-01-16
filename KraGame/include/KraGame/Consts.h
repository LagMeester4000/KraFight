#pragma once
#include <KraFight/TypeDef.h>

namespace game {
	const kra::kfloat FixPixelPerUnit = kra::kfloat::makeFromInt(64);
	const kra::kfloat FixPixelScale = kra::kfloat::makeFromFraction(1, 64);
	const float PixelPerUnit = 50.f / 50.f;
	const float PixelScale = 1.f / PixelPerUnit;
}