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

			// Insert input on specific frame 
			//   (if said frame is not too far in the past)
			// If the input is beyond the current highest frame,
			// Shift the ringbuffer forward
			void InsertInput(const KraNetInput& Input, i32 Frame);

			// Get an input from the buffer
			// If the given frame is invalid, repeat the last valid frame
			// If there is no last valid frame, return null input
			KraNetInput GetInput(i32 Frame) const;

			// Get the last valid input
			// If there is no last valid frame, return null input
			KraNetInput GetLastValidInput(i32 Frame) const;

			// Check if an input is valid
			bool IsInputValid(i32 Frame) const;
			
			// Check if a range of inputs is valid (including the EndFrame)
			bool IsInputRangeValid(i32 StartFrame, i32 EndFrame) const;

			// Get the current frame of the buffer
			// Equals the size of the buffer - 1
			i32 GetFrame() const;

			// Check if the buffer has any gaps
			// This could mean the buffer is missing an input frame
			bool CheckForGaps();

			/// Net
			// Put the highest x frames of input and put them in the packet
			void SaveToPacket(Packet& Pack);

			// Take old input and put it into an input packet
			void SaveOldInputToPacket(Packet& Pack, i32 Frame);

			// Put the input from a packet into the input buffer
			bool LoadFromPacket(const Packet& Pack);

		private:
			RingBufferStatic<Element, InputBufferSize> Inputs;
		};
	}
}