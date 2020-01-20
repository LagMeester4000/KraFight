#pragma once
#include "KraNet/Types.h"
#include "KraNet/InputBuffer.h"
#include "KraNet/Packet.h"
#include <SFML/Network/IpAddress.hpp>

namespace kra {
	namespace net {
		class Peer {
		public:
			void InitRemote(sf::IpAddress Ip, u16 Port, i8 PlayerNumber);
			void InitLocal(i8 PlayerNumber);

			const sf::IpAddress& GetIp() const;
			u16 GetPort() const;
			bool IsLocal() const;
			i8 GetPlayerNum() const;

		private: // Constants
			// Address
			sf::IpAddress Ip;
			u16 Port;

			// Local or not
			bool Local;

			// Player number
			i8 PlayerNumber;

		public: // Mutable
			// Inputs
			InputBuffer Inputs;
			
			// Last received packet from this peer
			Packet LastReceived;

			// Highest frame that has ever been sent by this peer
			u32 LastFrame = 0;

			// Last ping that has been sent to this peer
			u32 LastPing = 0;

			// Ping buffer
			RingBufferStatic<u32, PingBufferSize> PingOverTime;
		};
	}
}