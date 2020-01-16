#pragma once
#include <string>

namespace game {
	namespace str {
		// Returns the index of the last time character 'c' appears
		// On failure, returns the length of string "str"
		size_t FindLastInstanceOf(char Ch, const std::string& Str);

		// Returns the index of the first time character 'c' appears
		// On failure, returns the length of string "str"
		size_t FindFirstInstanceOf(char Ch, const std::string& Str);
	}
}