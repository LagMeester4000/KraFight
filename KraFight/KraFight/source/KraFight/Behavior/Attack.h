#pragma once
#include <vector>
#include <tuple>
#include <utility>
#include "KraFight/Detail/Function.h"
#include "KraFight/Context.h"
#include "KraFight/Detail/Pointer.h"

namespace kra {
	//struct holding the variables needed for an action in the attack
	struct AttackContext {
		Context Context;
		class Entity* Entity;
	};

	class AttackFrameAction {
	public:
		virtual void Execute(const AttackContext& Context) = 0;
		//virtual FunctionRaw<Pointer<AttackFrameAction>()> GetCreateFunc() = 0;
	};

	template<typename ... TT>
	class AttackFrameActionImpl : public AttackFrameAction {
	public:
		void Execute(const AttackContext& Context) override
		{
			ExecuteFinal(std::make_index_sequence<sizeof...(TT)>{}, Context);
		}

		template<size_t ... II>
		void ExecuteFinal(std::index_sequence<II...>, const AttackContext& Context)
		{
			Func.func(Context, std::get<II>(Args)...);
		}

		std::tuple<TT...> Args;
		Function<void(const AttackContext&, TT...)> Func;
	};

	class AttackFrame {
	public:
		template<typename ... TT>
		AttackFrame& Add(FunctionRaw<void(const AttackContext&, TT...)> Func, TT&&... Args)
		{
			Pointer<AttackFrameActionImpl<TT...>> Push = MakePointer<AttackFrameActionImpl<TT...>>();
			Push->Func.func = Func;
			Push->Args = { std::forward<TT>(Args)... };
			Actions.push_back(PointerCast<AttackFrameAction>(Push));
			return *this;
		}

		void Execute(const AttackContext& Context);

	private:
		std::vector<Pointer<AttackFrameAction>> Actions;
	};

	// Attack resource class
	//neutralA[0].add(&startHitbox, 1);
	class Attack {
	public:
		// Returns the AttackFrame at frame I
		// If the frame does not exist yet it will be created
		AttackFrame& operator[](size_t I);

		// Executes the actions in a single AttackFrame
		void ExecuteFrame(size_t I, const AttackContext& Context);

		// Returns the amount of AttackFrames
		size_t Size();

	private:
		std::vector<AttackFrame> Frames;
	};

}