#pragma once
#include <type_traits>



// Macro to check if T has function named "name" with arguments "..." and return "ret"
#define HAS_FUNC_NAMED_RET(funcName, name, ret, ...)																	\
namespace templates {																									\
																														\
	template<typename, typename, typename = int, typename ...>															\
	struct T##funcName : std::false_type { };																			\
																														\
	template<typename T, typename Ret, typename ...args>																\
	struct T##funcName<																									\
		T, Ret,																											\
		std::enable_if_t<std::is_same_v<Ret, decltype(std::declval<T>().name(std::declval<args>()...))>, int>,			\
		args...																											\
	> : std::true_type { };																								\
																														\
}																														\
																														\
template<typename T>																									\
static constexpr bool funcName = templates::##T##funcName<T, ret, int, __VA_ARGS__>::value;



// Macro to check if T has function named "name" with arguments "..."
#define HAS_FUNC_NAMED(funcName, name, ...)																				\
namespace templates {																									\
																														\
	template<typename T, typename = int, typename ...args>																\
	struct T##funcName : std::false_type {};																			\
																														\
	template<typename T, typename ...args>																				\
	struct T##funcName<T, decltype(std::declval<T>().name(std::declval<args>()...), 0), args...> : std::true_type {};	\
																														\
}																														\
																														\
template<typename T>																									\
static constexpr bool funcName = templates::##T##funcName<T, int, __VA_ARGS__>::value;



#define IS_RAW_FUNC_NAMED(funcName, name, ...)																				\
namespace templates {																									\
																														\
	template<typename T, typename = int, typename ...args>																\
	struct T##funcName : std::false_type {};																			\
																														\
	template<typename T, typename ...args>																				\
	struct T##funcName<T, decltype(name(std::declval<args>()...), 0), args...> : std::true_type {};	\
																														\
}																														\
																														\
template<typename T>																									\
static constexpr bool funcName = templates::##T##funcName<T, int, __VA_ARGS__>::value;