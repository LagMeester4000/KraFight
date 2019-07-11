#pragma once
#include "KraFight/Math/Vector2.h"
#include "HitProperties.h"

namespace kra {
	struct HitboxData {
		Vector2 Position;
		Vector2 Size;
		HitProperties HitProps;
	};
}