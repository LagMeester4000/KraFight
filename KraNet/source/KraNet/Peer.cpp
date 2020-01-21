#include "KraNet/Peer.h"

void kra::net::Peer::InitRemote(sf::IpAddress AIp, u16 APort, i8 APlayerNumber)
{
	Ip = AIp;
	Port = APort;
	Local = false;
	PlayerNumber = APlayerNumber;
}

void kra::net::Peer::InitLocal(i8 APlayerNumber)
{
	Ip = 0;
	Port = 0;
	Local = true;
	PlayerNumber = APlayerNumber;
}

const sf::IpAddress & kra::net::Peer::GetIp() const
{
	return Ip;
}

u16 kra::net::Peer::GetPort() const
{
	return Port;
}

bool kra::net::Peer::IsLocal() const
{
	return Local;
}

i8 kra::net::Peer::GetPlayerNum() const
{
	return PlayerNumber;
}
