#pragma once
#include "KraNetHeader.h"
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <vector>

namespace kra {
	namespace net {
		struct Packet;

		class UDPSocket {
		public:
			UDPSocket();

			// Initialize the socket
			void Init(u16 Port);

			// Add a local player number
			void RegisterLocaLPlayer(i8 Num);

			// Check if this is a local player
			bool IsLocalPlayer(i8 Num);

			// Receive data from the network
			// Returns true if data has been received, false otherwise
			bool Receive(Packet& Pack, sf::IpAddress& Ip, u16& Port, KraNetHeader& Header);
			bool Receive(Packet& Pack, sf::IpAddress& Ip, u16& Port);
			bool Receive(Packet& Pack, KraNetHeader& Header);

			// Send data throught the buffer
			bool Send(const Packet& Pack, i8 ThisPlayer, const sf::IpAddress& Ip, u16 Port, i8 OtherPlayer);
			
			// Send a packet from all local players
			bool SendFromAll(const Packet& Pack, const sf::IpAddress& Ip, u16 Port, i8 OtherPlayer);

		private:
			// List of local players
			std::vector<i8> LocalPlayers;

			// SFML UDP socket
			sf::UdpSocket Socket;

			// Cached packet
			// Reused to remove some allocation time
			sf::Packet CachedPacket;
		};
	}
}