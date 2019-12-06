#pragma once

namespace kra {
	template<typename T>
	struct Function {
		T* func = nullptr;
	};

	template<typename T>
	using FunctionRaw = T*;
}