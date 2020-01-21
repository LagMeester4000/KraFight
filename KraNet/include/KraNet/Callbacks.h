#pragma once
#include "Types.h"

struct KraNetInput;
namespace kra {
	namespace net {
		template<typename T>
		using Func = T*;

		struct Callbacks {
			Func<void(void*, KraNetInput*, u32)> UpdateFunc;
			Func<void(void*, KraNetInput*, u32)> SimulateFunc;
			Func<void(void*)> SaveState;
			Func<void(void*)> LoadState;
		};
	}
}