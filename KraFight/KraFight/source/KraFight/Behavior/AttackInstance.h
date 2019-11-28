#pragma once
#include "KraFight/Detail/Handle.h"
#include "KraFight/TypeDef.h"
#include "KraFight/Consts.h"

namespace kra {
	class Attack;
	struct AttackContext;

	class AttackInstance {
	public:
		AttackInstance();

		// Update the attack
		void Update(kfloat DeltaTime, const AttackContext& Con);

		// Activate the attack instance
		void Activate(Handle<Attack> NewAttack);

		// Returns Active bool
		// Can be checked to see if attack has ended
		bool IsActive() const;

	private:
		bool Active;
		kfloat Timer;
		Handle<Attack> CurrentAttack;
	};
}