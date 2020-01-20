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
			u16 Type = 0;

			// Actual data in the packet
			union {
				struct {

				} Connect;

				struct {
					u32 Frame;
					u32 InputSize;
					KraNetInput Inputs[PacketInputBufferSize];
				} GameInput;

				struct {
					u32 Frame;
				} MissedGameInput;
				
				struct {
					u32 TimeAsMs;
				} PingRequest;

				struct {
					u32 TimeAsMs;
					u32 Frame; // Sends back the current frame
				} PingResponse;

			} Pack;
		};
	}
}