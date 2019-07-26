#pragma once
#include "HitId.h"

namespace kra {
	class HitIdGenerator {
	public:
		HitIdGenerator();

		// Generate a new HitId
		// Increases Counter value
		HitId Generate();

	private:
		HitIdT Counter = 1;
	};
}