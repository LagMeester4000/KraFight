#pragma once
#include "Consts.h"
#include "Peer.h"
#include <memory>
#include <vector>

namespace sf {
	class Packet;
}

namespace kra {
	class UDPSocket;
	struct Packet;
	struct KraNetHeader;

	namespace net {
		class PeerManager {
		public:
			PeerManager(std::shared_ptr<UDPSocket> Socket, u32 PlayerAmount, bool Spectator);

			// Process a packet
			void ProcessPacket(i8 From, const Packet& Pack);

			// Send a packet to all of the peers
			// Example: sending input data
			void SendToAll(const Packet& Pack);
			
			// Add a new peer
			void AddPeer(i8 PlayerNum, const sf::IpAddress& Ip, u16 Port);
			void AddLocalPeer(u16 Port, i8 PlayerNum);

			// Get the other peers
			std::vector<Peer>& GetPeers();

			// Get a single Peer
			Peer& GetPeer(i8 PlayerNum);

			// Get (the first) local peer
			Peer& GetLocalPeer();

		private:
			// Main socket (not created by this class)
			std::shared_ptr<UDPSocket> Socket;

			// Peers
			std::vector<Peer> Peers;

			// The amount of players that will be in the game
			u32 AmountOfPlayers;

			// True if this client a spectator
			bool Spectator;
		};
	}
}