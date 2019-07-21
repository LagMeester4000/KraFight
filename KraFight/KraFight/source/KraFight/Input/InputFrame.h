#pragma once
#include <inttypes.h>

namespace kra {
	struct Button {
		bool Held = false;
		bool Consumed = false;
	};

	struct InputFrame {
		// Stick
		bool StickX = false;
		bool StickXNotNull = false;
		bool StickY = false;
		bool StickYNotNull = false;
		bool StickConsumed = false;

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