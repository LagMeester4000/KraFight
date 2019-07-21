#pragma once
#include "InputBuffer.h"
#include "KraFight/Detail/Handle.h"
#include "InputFrame.h"
#include <array>

namespace kra {
	class InputManager {
	public:
		InputManager();

		// Called at the end of a frame
		void PushDown();

		// Edit the current input frame
		InputFrame& Edit(Handle<InputBuffer> InputHandle);

		// Get a reference to the input buffer to check for inputs
		const InputBuffer& Get(Handle<InputBuffer> InputHandle);

	private:
		std::array<InputBuffer, 2> PlayerInput;
	};
}