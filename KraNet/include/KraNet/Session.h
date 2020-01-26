#pragma once
#include "PeerManager.h"
#include "Address.h"
#include "Callbacks.h"
#include "Settings.h"
#include <vector>
#include <SFML/System/Clock.hpp>

namespace kra {
	namespace net {
		class Session {
		public:
			Session(u16 LocalPort, u32 PlayerAmount, bool Spectator);
			
			// Add a player to the game session
			// Can only be called when the session is still being set up
			void AddPlayer(i8 PlayerNumber, Address Addr);
			void AddLocalPlayer(i8 PlayerNumber, u16 Port);

			// Add a spectator to the game session
			// DOES NOT WORK AT THE MOMENT
			void AddSpectator(i8 SpectatorNumber, Address Addr);

			// Start the session and set up the connections
			// After this, no players or spectaters can be added
			void Start();

			// Update the session with a local input
			// The session can choose to ignore the input if it needs to
			// This function only needs to be called after `Start` has been called
			void Update(const std::vector<KraNetInput>& Input);

			// Set the callbacks
			void SetCallbacks(void* External, Callbacks Callback);

		private:
			void Receive();
			void SendInput();
			void UpdateInput(const std::vector<KraNetInput>& Input);
			void TryRollback();
			void UpdateGame();
			void ProcessPacket(const KraNetHeader& Header, const Packet& Pack);

			// Get the current gameplay frame
			// This function is only valid in a frame after where
			//   the UpdateInput() function would be called
			//   So that would be in the Receive, Send, Rollback,
			//   and UpdateGame functions
			i32 GetCurrentFrame();

		private:
			std::shared_ptr<UDPSocket> MainSocket;
			PeerManager Peers;
			bool Started = false;
			
			// Callback
			Callbacks Callback;
			void* External;

			// Main settings for the netplay
			Settings Setts;

			// To check if this frame an update will be performed
			bool UpdateFrame = false;

		private: // Rollback
			i32 LastConfirmedFrame = 0;
			sf::Clock PingClock;
		};
	}
}