#pragma once
#include "Types.h"

namespace kra {
	namespace net {
		struct PingTest {
			// Last checked ping
			// Stored in milliseconds round-trip-time
			u32 Ping;

			// Frame difference obtained during last ping check
			// Saved as float to preserve precision
			f32 LocalFrameDiff;
		};
	}
}