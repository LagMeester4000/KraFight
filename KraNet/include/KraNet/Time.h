#pragma once
#include "Types.h"

namespace kra {
	namespace net {
		// Class that keeps time relative to the start of the program/object
		class Time {
		public:
			Time();

			// Reset the clock
			void Reset();

			// Get time since the 
			u32 GetCurrentTime();
		};
	}
}