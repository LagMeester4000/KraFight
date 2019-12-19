#pragma once
#include "InputFrame.h"
#include <array>

namespace kra {
	class NetSaveBuffer;
	class NetLoadBuffer;

	class InputBuffer {
		static const int BufferSize = 128;
		static const int DefaultDepth = 8;
	public:
		InputBuffer();

		// Return the latest frame
		InputFrame& Edit();

		// Push all the inputs down a frame
		void PushDown();

		// Check if a button has been pressed
		bool Pressed(Button InputFrame::* Button, int Depth = DefaultDepth, bool CheckForConsume = true) const;

		// Check if a button has been released
		bool Release(Button InputFrame::* Button, int Depth = DefaultDepth, bool CheckForConsume = true) const;
		
		// Check if a button has been held
		bool Held(Button InputFrame::* Button, int Depth = DefaultDepth, bool CheckForConsume = true) const;

		// Check for the current stick X
		int StickX(int Depth = 0) const;

		// Check for the current stick Y
		int StickY(int Depth = 0) const;

		// Consume the input of a button
		void Consume(Button InputFrame::* Button);

		// Consume the stick
		void ConsumeStick();

	public: // Networking
		void NetSave(NetSaveBuffer& Buff);
		void NetLoad(NetLoadBuffer& Buff);

	private:
		std::array<InputFrame, BufferSize> Inputs;
	};
}