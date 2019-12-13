#pragma once
#include <vector>
#include <chrono>
#include <SFML/Network.hpp>
#include "NetInputBuffer.h"
#include "NetPacket.h"

namespace kra {
	namespace net {
		class KraNetSession {
		public:
			using Clock = std::chrono::high_resolution_clock;
			using TimePoint = std::chrono::time_point<Clock>;
			
		public:
			KraNetSession(void* ExternalData, void(*UpdateF)(void*, KraNetInput, KraNetInput));
			~KraNetSession();
			
			// Client
			bool StartConnection(sf::IpAddress OtherIp, unsigned short Port);

			// Server
			bool ListenConnection(unsigned short Port);

			// Update the inputs
			void Update(KraNetInput LocalInput);

			// Get Host value
			bool IsHost() const;

		public: // Getters
			double GetPing() const;
			sf::IpAddress GetOtherIp() const;
			unsigned short GetOtherPort() const;
			unsigned short GetDelay() const;
			uint32_t GetPingIndex() const;
			int64_t GetLastFrameDifference() const;

		private:
			void Initialize();

			void UpdateGameplay();
			void UpdateLocalInput(KraNetInput LocalInput);
			void SendInput();
			void ReceiveInput();

		private: // Const data
			// Pointer to some external class/object
			void* External;

			// Function that is called every frame to update the game
			void(*UpdateFunction)(void*, KraNetInput, KraNetInput);

			// The ip to connect to
			sf::IpAddress OtherIp;

			// Port for connection
			unsigned short OtherPort;
			
			// The input delay in frames
			unsigned short DelayLength;

			// Holds if this peer is the host
			bool Host;

		private: // Non-const data
			NetInputBuffer Inputs;
			sf::UdpSocket Sock;
			sf::Packet InPacket, OutPacket;
			NetPacket InNetPacket, OutNetPacket;

			// Ping
			TimePoint LastPing;
			uint32_t PingIndex = 1; // Very important to start at 1
			bool PingDone = false;
			double LastPingTime = 0.0;
			int64_t LastFrameDifference = 0;
		};
	}
}