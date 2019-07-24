#pragma once
#include "KraFight/Detail/Function.h"
#include "KraFight/Detail/Handle.h"
#include "KraFight/TypeDef.h"
#include <map>
#include <vector>
#include <array>

namespace kra {
	struct Context;

	// T = Base type
	// E = State enum type
	// MaxEnum = The highest value of the enum
	// State Machine class
	// Used to manage switching between states more smoothely
	// 
	// System update order: Entity -> __StateMachine__ -> Hitbox -> Physics
	// This is when in the (a single) frame the state machine is updated
	//
	// State machine action order: Condition -> Leave -> Enter -> Update
	// This is the order in which the functions in the state machine are called
	// If the condition does not result in true, the Leave and Enter functions won't be called, obviously
	template<typename T, typename E, E MaxEnum>
	class StateMachine {
		friend class StateMachineSetup<T, E, MaxEnum>;

		// Struct with information for a state switch
		struct Condition {
			E To;
			Function<bool(const Context&, Handle<T>)> Func;
		};

		using OnSwitchList = std::array<std::vector<Function<void(const Context&, Handle<T>)>>, (size_t)MaxEnum>;
		using OnUpdateList = std::array<std::vector<Function<void(const Context&, kfloat, Handle<T>)>>, (size_t)MaxEnum>;
		using ConditionList = std::map<E, std::vector<Condition>>;
		
	public:
		StateMachine(E DefaultState)
			: CurrentState(DefaultState), PreviousState(DefaultState)
		{
		}

		// Set the owner of the state machine
		void SetOwner(Handle<T> Hand)
		{
			Owner = Hand;
		}

		// Update the whole state machine
		void Update(const Context& Con, kfloat DeltaTime)
		{
			// Conditions
			auto& Conds = Conditions[(size_t)CurrentState];
			for (auto& It : Conds)
			{
				// Check condition
				if (It.Func.func(Con, Owner))
				{
					SetState(It.To, Con);
					break;
				}
			}

			// Update funcs
			auto& Updates = OnUpdate[(size_t)CurrentState];
			for (auto& It : Updates)
			{
				Updates.func(Con, DeltaTime, Owner);
			}
		}
		
		// Forces the state to be set to NewState
		// This should not be called most of the time
		// Examples of use: On Get Hit, 
		void ForceSetState(E NewState, const Context& Con)
		{
			SetState(NewState, Con);
		}

		// Get the current state value
		E GetCurrentState() const
		{
			return CurrentState;
		}

		// Get the previous state value
		E GetPreviousState() const
		{
			return PreviousState;
		}

	private:
		void SetState(E NewState, const Context& Con)
		{
			PreviousState = CurrentState;
			CurrentState = NewState;

			auto& Leaves = OnLeave[(size_t)PreviousState];
			for (auto& It : Leaves)
			{
				It.func(Con, Owner);
			}

			auto& Enters = OnLeave[(size_t)CurrentState];
			for (auto& It : Enters)
			{
				It.func(Con, Owner);
			}
		}

	private:
		E CurrentState, PreviousState;
		Handle<T> Owner;

		// Funcs
		OnSwitchList OnEnter, OnLeave;
		OnUpdateList OnUpdate;
		ConditionList Conditions;
	};

	// Class used to set up the state machine object
	template<typename T, typename E, E MaxEnum>
	class StateMachineSetup {
	public:
		StateMachineSetup(StateMachine<T, E, MaxEnum>& Ref) 
			: SM(Ref) 
		{}

		// Add a condition for switching to another state
		void AddCondition(E From, E To, Function<bool(const Context&, Handle<T>)> Func)
		{
			auto NewCond = typename StateMachine<T, E, MaxEnum>::Condition();
			NewCond.To = To;
			NewCond.Func = Func;
			SM.Conditions[From].push_back(NewCond);
		}

		// Add a function that is called when entering `To` state
		void AddOnEnter(E To, Function<void(const Context&, Handle<T>)> Func)
		{
			SM.OnEnter[To].push_back(Func);
		}

		// Add a function that is called when leaving `From` state
		void AddOnLeave(E From, Function<void(const Context&, Handle<T>)> Func)
		{
			SM.OnLeave[From].push_back(Func);
		}

		// Add a function that is called each frame
		void AddUpdate(E For, Function<void(const Context&, kfloat, Handle<T>)> Func)
		{
			SM.OnUpdate[For].push_back(Func);
		}

	private:
		StateMachine<T, E, MaxEnum>& SM;
	};
}