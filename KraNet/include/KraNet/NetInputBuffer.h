#pragma once
#include <vector>
#include <stdint.h>
#include "KraNetInput.h"

namespace kra {
	namespace net {
		struct InputPair {
			KraNetInput Local, Remote;
		};

		struct NetInputBufferElement {
			InputPair Input;
			bool LocalValid = false,
				RemoteValid = false;
		};

		// Circlebuffer for storing inputs by frame
		class NetInputBuffer {
		public:
			using FrameT = uint32_t;
		public:
			NetInputBuffer();

			// Reset the buffer
			// Sets CurrentFrame to 0
			void Reset();

			// Check if you can advance the buffer by one frame
			bool CanAdvanceGameplayFrame() const;

			// Advance the buffer by one frame
			bool AdvanceGameplayFrame();

			// Get input for current gameplay frame
			const InputPair& GetGameplayFrame() const;

			// Insert input from network
			void InsertNetworkFrame(FrameT NewFrame, KraNetInput Input);

			// Insert local input
			void InsertLocalFrame(FrameT Delay, KraNetInput Input);

			// Initialize the local frames to account for buffer
			// Should only be used once at the start of the session
			void InitializeLocalFrames(FrameT Delay);

			// Set the size of the input buffer
			// Should only be used once at the start of the program
			void SetSize(size_t NewSize);

			// Get an input from the buffer
			const InputPair& GetInput(FrameT Frame) const;

			// Check if the frame is valid
			// Valid meaning that the frame still exists in the buffer and is initialized
			bool IsValid(FrameT Frame) const;

			// Check if the frame is in range of the buffer
			bool IsInRange(FrameT Frame) const;

			// Returns the lowest accessible frame in the buffer
			FrameT LowestFrame() const;
			FrameT GetGameplayFrameIndex() const;

			// Make a sendable buffer
			void MakeSendableInputBuffer(std::vector<KraNetInput>& Buff, FrameT& StartingFrame) const;

			// Read a received buffer
			void ReadReceivedInputBuffer(const std::vector<KraNetInput>& Buff, FrameT StartingFrame);

		private:
			// Returns wether the buffer is initialized
			bool BufferValid() const;

			// Resize the Inputs array but move the elements to their new correct spot
			void ResizeAndFit(size_t NewSize);

			// Get a reference to an element using a frame (does modulo)
			NetInputBufferElement& Access(size_t Frame);
			const NetInputBufferElement& AccessConst(size_t Frame) const;
			static NetInputBufferElement& Access(std::vector<NetInputBufferElement>& Inputs, size_t Frame);

			// Helper
			static FrameT Max(FrameT f1, FrameT f2);

		private:
			std::vector<NetInputBufferElement> Inputs;
			FrameT CurrentGameplayFrame;
			FrameT MaxLocalFrame;
			FrameT MaxNetworkFrame;
		};
	}
}