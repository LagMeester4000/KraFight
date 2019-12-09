#pragma once

namespace kra {
	// Object holding an optional value
	// If `Exists` boolean is false, the object does not exist
	template<typename T>
	struct Optional {
		T Value;
		bool Exists = false;
	};
}