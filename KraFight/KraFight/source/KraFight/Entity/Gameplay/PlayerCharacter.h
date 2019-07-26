#pragma once
#include "PlayerAttributes.h"
#include "KraFight/Entity/Entity.h"
#include "KraFight/Detail/Handle.h"
#include "KraFight/Behavior/StateMachineDef.h"

namespace kra {
	class InputBuffer;

	class PlayerCharacter : public Entity {
	public:
		PlayerCharacter();

		void SetupPlayer(Handle<InputBuffer> Input, int PlayerNum);

		// Update state machine and the sorts
		void Update(kfloat DeltaTime, const Context& Con) override;

		// Setup the states in the state machine
		virtual void SetupStateMachine(StateMachineSetup& Setup);

	protected:
		Handle<InputBuffer> InputHandle;
		PlayerAttributes Attributes;
		int PlayerNumber;
	};
}