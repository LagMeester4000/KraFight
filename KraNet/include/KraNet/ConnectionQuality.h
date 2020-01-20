#pragma once
#include "Types.h"

namespace kra {
	namespace net {
		struct ConnectionQuality {
			// Ping obtained
			u32 Ping;

			// Difference between our local frame and remote frame
			i32 FrameDiff;
		};
	}
}