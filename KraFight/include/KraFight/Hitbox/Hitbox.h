#pragma once
#include "Box.h"
#include "HitProperties.h"

namespace kra {
	struct Hitbox : Box {
		HitProperties HitProps;
		bool bActive;
	};
}