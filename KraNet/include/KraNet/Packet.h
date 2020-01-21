#pragma once
#include "Consts.h"
#include "KraNetInput.h"

namespace sf {
	class Packet;
}

namespace kra {
	namespace net {
		struct PacketType {
			enum {
				None = 0,
				Connect,
				GameInput,
				MissedGameInput,
				PingRequest,
				PingResponse,
			};
		};

		// Struct holding any data sent by a packet
		struct Packet {
			void Load(sf::Packet& Pack);
			void Save(sf::Packet& Pack) const;

			// Indication for which union value to use
			u16 Type;

			// Actual data in the packet
			union P {
				P() {}
				~P() {}

				struct {
					bool IsConnectedToAllPeers;
				} Connect;

				struct {
					i32 Frame;
					u32 InputSize;
					KraNetInput Inputs[PacketInputBufferSize];
				} GameInput;

				struct {
					i32 Frame;
				} MissedGameInput;
				
				struct {
					u32 TimeAsMs;
				} PingRequest;

				struct {
					u32 TimeAsMs;
					i32 Frame; // Sends back the current frame
				} PingResponse;

			} Pack;
		};
	}
}