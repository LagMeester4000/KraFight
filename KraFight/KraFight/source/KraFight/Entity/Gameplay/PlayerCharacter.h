#pragma once
#include "../Entity.h"
#include "KraFight/Detail/Handle.h"

namespace kra {
	class InputBuffer;

	class PlayerCharacter : public Entity {
	public:
		PlayerCharacter();

		void SetupPlayer(Handle<InputBuffer> Input);

		// Update state machine and the sorts
		void Update(kfloat DeltaTime, const Context& Con) override;

		// Setup the states in the state machine
		virtual void SetupStateMachine();


	protected:
		Handle<InputBuffer> InputHandle;
	};
}