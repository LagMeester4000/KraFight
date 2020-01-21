#pragma once
#include "Types.h"

namespace kra {
	namespace net {
		struct Settings {
			// The set amount of input delay
			i32 InputDelay = 0;

			// The maximum amount of rollback in frames
			i32 MaxRollback = 7;
		};
	}
}