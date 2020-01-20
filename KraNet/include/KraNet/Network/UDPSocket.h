#pragma once
#include "KraNetHeader.h"
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>

namespace kra {
	namespace net {
		struct Packet;

		class UDPSocket {
		public:
			UDPSocket();

			// Initialize the socket
			void Init(u16 Port);

			// Set the player number
			void SetPlayerNumber(i8 Num);

			// Receive data from the network
			// Returns true if data has been received, false otherwise
			bool Receive(Packet& Pack, sf::IpAddress& Ip, u16& Port, KraNetHeader& Header);
			bool Receive(Packet& Pack, sf::IpAddress& Ip, u16& Port);

			// Send data throught the buffer
			bool Send(const Packet& Pack, const sf::IpAddress& Ip, u16 Port, i8 OtherPlayer);

		private:
			// Local header to be used when a message is sent
			KraNetHeader LocalHeader;

			// SFML UDP socket
			sf::UdpSocket Socket;

			// Cached packet
			// Reused to remove some allocation time
			sf::Packet CachedPacket;
		};
	}
}