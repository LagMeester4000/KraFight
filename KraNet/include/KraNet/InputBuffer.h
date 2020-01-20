#pragma once
#include "KraNetInput.h"
#include "Consts.h"
#include "Detail/RingBufferStatic.h"

namespace kra {
	namespace net {
		struct Packet;

		// Class that stores inputs 
		class InputBuffer {
		public:
			struct Element {
				bool Valid = false;
				KraNetInput Input;
			};

		public:
			InputBuffer();

			// Push a new input into the buffer
			void PushInput(const KraNetInput& Input);

			// Get an input from the buffer
			const KraNetInput& GetInput(u32 Frame) const;

			// Check if an input is valid
			bool IsInputValid(u32 Frame) const;
			
			// Check if a range of inputs is valid (including the EndFrame)
			bool IsInputRangeValid(u32 StartFrame, u32 EndFrame) const;

			// Get the current frame of the buffer
			// Equals the size of the buffer - 1
			u32 GetFrame() const;

			// Check if the buffer has any gaps
			// This could mean the buffer is missing an input frame
			bool CheckForGaps();

			// Net
			void SaveToPacket(Packet& Pack);
			bool LoadFromPacket(const Packet& Pack);

		private:
			RingBufferStatic<Element, InputBufferSize> Inputs;
		};
	}
}