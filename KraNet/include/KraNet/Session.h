#pragma once
#include "PeerManager.h"
#include "Address.h"
#include <vector>

namespace kra {
	namespace net {
		class Session {
		public:
			Session(u16 LocalPort, u32 PlayerAmount, bool Spectator);

			// Add a player to the game session
			// Can only be called when the session is still being set up
			void AddPlayer(i8 PlayerNumber, Address Addr);

			// Add a spectator to the game session
			// DOES NOT WORK AT THE MOMENT
			void AddSpectator(i8 SpectatorNumber, Address Addr);

			// Start the session and set up the connections
			// After this, no players or spectaters can be added
			void Start();

			// Update the session with a local input
			// The session can choose to ignore the input if it needs to
			void Update(KraNetInput Input);

		private:
			void Receive();
			void SendInput();

		private:
			PeerManager Peers;
			std::shared_ptr<UDPSocket> Socket;
			bool Started = false;
		};
	}
}