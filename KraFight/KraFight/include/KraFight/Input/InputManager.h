#pragma once
#include "InputBuffer.h"
#include "KraFight/Detail/Handle.h"
#include "InputFrame.h"
#include <array>

namespace kra {
	class InputManager {
	public:
		InputManager();

		// Update the input manager
		void Update();

		// Edit the current input frame
		InputFrame& Edit(Handle<InputBuffer> InputHandle);

		// Insert a new input frame
		// Pushes all old frames down
		void Insert(Handle<InputBuffer> InputHandle, const InputFrame& Frame);

		// Get a reference to the input buffer to check for inputs
		const InputBuffer& Get(Handle<InputBuffer> InputHandle) const;

		// Set if the input should be changed
		void SetInputEnabled(bool bVal);

		// Returns if input is enabled
		bool IsInputEnabled() const;

		// Make a new handle to access the input manager
		Handle<InputBuffer> MakeHandle(int PlayerIndex) const;

	private:
		// Called at the end of a frame
		void PushDown();

	private:
		std::array<InputBuffer, 2> PlayerInput;
		bool bInputEnabled = false;
	};
}