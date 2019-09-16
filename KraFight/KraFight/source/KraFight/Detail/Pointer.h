#pragma once
#include <memory>

namespace kra {
	template<typename T>
	using Pointer = std::shared_ptr<T>;

	template<typename T, typename ... TT>
	Pointer<T> MakePointer(TT&& ... t)
	{
		return std::make_shared<T>(std::forward<TT>(t)...);
	}

	template<typename T, typename U>
	Pointer<T> PointerCast(Pointer<U>& Ptr)
	{
		return std::static_pointer_cast<T>(Ptr);
	}

	template<typename T, typename U>
	Pointer<T> PointerCast(Pointer<U>&& Ptr)
	{
		return std::static_pointer_cast<T>(Ptr);
	}

	template<typename T, typename U>
	Pointer<T> PointerDynCast(Pointer<U>& Ptr)
	{
		return std::dynamic_pointer_cast<T>(Ptr);
	}

	template<typename T, typename U>
	Pointer<T> PointerDynCast(Pointer<U>&& Ptr)
	{
		return std::dynamic_pointer_cast<T>(Ptr);
	}
}