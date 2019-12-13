#pragma once
#include <stdint.h>
#include "KraNetInput.h"
#include "NetInputBuffer.h"

namespace sf {
	class Packet;
}

namespace kra {
	namespace net {
		enum class NetPacketType : uint8_t {
			Error,
			Setup,
			Input,
			Desync,
		};

		class NetPacket {
		public:
			NetPacket();

			void Load(sf::Packet& Pack);
			void Save(sf::Packet& Pack);

		public:
			NetPacketType Type;

			struct { // Error

			} Error;

			struct { // Setup

			} Setup;

			struct { // Input
				// Ping
				uint32_t PingIndexIn = 0, PingIndexOut = 0;
				uint32_t GameplayFrame = 0;

				// Input
				NetInputBuffer::FrameT StartFrame;
				std::vector<KraNetInput> Input;
			} Input;

			struct { // Desync

			} Desync;
		};
	}
}