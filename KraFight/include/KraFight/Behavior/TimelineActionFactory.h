#pragma once
#include "KraFight/Detail/Function.h"
#include "KraFight/Detail/Pointer.h"
#include <string>
#include <map>

namespace kra {
	class TimelineAction;

	// Struct that holds information to create a timeline action
	struct TimelineActionFactoryElement {
		Function<Pointer<TimelineAction>(void)> CreateFunction;
	};

	// Resource that holds all the registered timeline actions
	class TimelineActionFactory {
	public:
		TimelineActionFactory();

		// Register a type into the factory
		template<typename T>
		void Register(const char* Name);

		// Create a new timeline action from the factory
		Pointer<TimelineAction> Create(const std::string& Name);

		// Get reference to map containing all elements
		const std::map<std::string, TimelineActionFactoryElement>& GetAllElements();

	private:
		std::map<std::string, TimelineActionFactoryElement> Elements;
	};

	template<typename T>
	inline void TimelineActionFactory::Register(const char * Name)
	{
		auto Func = []() {
			return PointerCast<TimelineAction>(MakePointer<T>());
		};
		TimelineActionFactoryElement Push;
		Push.CreateFunction = Func;
		Elements[Name] = Push;
	}
}