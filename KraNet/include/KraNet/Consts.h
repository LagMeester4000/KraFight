#pragma once
#include "Types.h"

namespace kra {
	namespace net {
		// Magic number used to identify packets
		const u32 MagicNum = 2143658709;
		
		// Max amount of inputs sent in a packet
		const u32 PacketInputBufferSize = 10;
		
		// Max amount of players in a multiplayer game
		const u32 MaxPlayers = 4;
		
		// The size of the input buffer stored for each player
		const u32 InputBufferSize = 100;

		// The amount of time before a ping times out and another one has to be sent
		const u32 PingTimeout = 1000;

		// Size of the buffer that stores ping
		const u32 PingBufferSize = 10;

		// FPS of the game
		const u32 FramesPerSecond = 60;
	}
}