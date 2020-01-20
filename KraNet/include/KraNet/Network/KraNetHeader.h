#pragma once
#include "KraNet/Consts.h"

namespace sf {
	class Packet;
}

namespace kra {
	namespace net {
		// Header for any KraNet packet
		struct KraNetHeader {
			bool Load(sf::Packet& Pack);
			void Save(sf::Packet& Pack) const;

			// Magic number to identify as KraNet packet
			u32 MagicNumber = 0;

			// Index of the player that sent this packet
			// Equals -1 if number is unknown
			// Equals -2 if spectator
			i8 FromPlayer = -3;

			// Index of the player that should receive this packet
			// The reason this is in the header is so that a server 
			//   could pick it up and redirect it to the correct player
			//   if it was doing relay (how else would it know)
			// Equals -1 if number is unknown
			// Equals -2 if spectator
			i8 ToPlayer = -3;
		};
	}
}