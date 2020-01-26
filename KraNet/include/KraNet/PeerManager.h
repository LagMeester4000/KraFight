#pragma once
#include "Consts.h"
#include "Peer.h"
#include <memory>
#include <vector>

namespace sf {
	class Packet;
}

namespace kra {

	namespace net {
		struct Packet;
		struct KraNetHeader;
		class UDPSocket;
		struct Settings;

		class PeerManager {
		public:
			PeerManager(std::shared_ptr<UDPSocket> Socket, u32 PlayerAmount, bool Spectator);

			// This probably doesnt belong in this class
			//// Process a packet
			//void ProcessPacket(const KraNetHeader& Header, const Packet& Pack);

			// Send a packet to all of the peers
			// Example: sending input data
			void SendToAll(const Packet& Pack, i8 FromLocalPlayerNum);
			
			// Add a new peer
			void AddPeer(i8 PlayerNum, const sf::IpAddress& Ip, u16 Port);
			void AddLocalPeer(u16 Port, i8 PlayerNum);

			// Get the other peers
			std::vector<Peer>& GetPeers();

			// Get a single Peer
			Peer* GetPeer(i8 PlayerNum);

			// Get (the first) local peer
			Peer* GetLocalPeer();

			// Get array of all local peers
			// TODO: improve function performance
			std::vector<std::reference_wrapper<Peer>> GetLocalPeers();

			// Returns if the peers are ready for gameplay
			// Meaning that all the peers have connected to each other
			bool ArePeersReady() const;

			// Returns if the peers are connected to you
			bool ArePeersConnected() const;

			// Get the current local frame
			i32 GetLocalFrame(const Settings& Sett);

			// Get the last local frame with input
			i32 GetLastLocalInputFrame();

			// Make an input array for a specific frame
			std::vector<KraNetInput> MakeInputFrame(i32 Frame);

			// Check if inputs for this frame has been received from all clients
			bool IsFrameConfirmed(i32 Frame);

		private:
			// Main socket (not created by this class)
			std::shared_ptr<UDPSocket> Socket;

			// Peers
			std::vector<Peer> Peers;

			// The amount of players that will be in the game
			u32 AmountOfPlayers;

			// True if this client a spectator
			bool Spectator;

			//// The current frame of the local peers and the game in general
			//i32 LocalFrame;
		};
	}
}