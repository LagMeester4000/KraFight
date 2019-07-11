#pragma once
#include <memory>

namespace kra {
	template<typename T>
	using Pointer = std::shared_ptr<T>;

	template<typename T, typename ... TT>
	Pointer<T> MakePointer(TT&& ... t)
	{
		return std::make_shared<T>(std::forward(t)...);
	}
}