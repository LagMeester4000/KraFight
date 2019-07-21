#pragma once
#include "KraFight/Detail/Function.h"
#include <map>
#include <vector>

namespace kra {
	// T = Base type
	// E = State enum type
	template<typename T, typename E>
	class StateMachine {
	public:
		StateMachine(E DefaultState)
			: CurrentState(DefaultState), PreviousState(DefaultState)
		{
		}
		
		void SetState(E NewState)
		{

		}

		E GetCurrentState() const
		{
			return CurrentState;
		}

		E GetPreviousState() const
		{
			return PreviousState;
		}


	private:
		E CurrentState, PreviousState;
		//std::map<E, >
	};
}