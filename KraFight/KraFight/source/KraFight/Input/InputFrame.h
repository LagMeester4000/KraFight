#pragma once
#include <inttypes.h>

namespace kra {
	struct Button {
		bool Held = false;
		bool Consumed = false;
	};

	struct InputFrame {
		// Stick
		int32_t StickX : 1;
		int32_t StickXNotNull : 1;
		int32_t StickY : 1;
		int32_t StickYNotNull : 1;

		// Buttons
		Button Attack1;
		Button Attack2;
		Button Attack3;
		Button Special;
		Button Grab;
	};

	// Not sure if working
	// Function to convert an input frame into an int
	int32_t ToInt(const InputFrame& In);

	// Not sure if working
	// Function to convert an int value into an input frame (without consume bool)
	InputFrame FromInt(int32_t In);
}