#include "KraNet/Network/KraNetHeader.h"
#include <SFML/Network/Packet.hpp>

bool kra::net::KraNetHeader::Load(sf::Packet & Pack)
{
	Pack >> MagicNumber;
	Pack >> FromPlayer;
	Pack >> ToPlayer;

	return MagicNumber == MagicNum;
}

void kra::net::KraNetHeader::Save(sf::Packet & Pack) const
{
	Pack << MagicNumber;
	Pack << FromPlayer;
	Pack << ToPlayer;
}
