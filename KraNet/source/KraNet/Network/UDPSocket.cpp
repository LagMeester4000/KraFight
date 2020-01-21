#include "KraNet/Network/UDPSocket.h"
#include "KraNet/Packet.h"
#include <assert.h>

kra::net::UDPSocket::UDPSocket()
{
}

void kra::net::UDPSocket::Init(u16 Port)
{
	// Set socket options:
	//TODO: switch to library fork that can do this^

	// Bind socket
	Socket.bind(Port);

	// Set non binding
	Socket.setBlocking(false);
}

void kra::net::UDPSocket::RegisterLocaLPlayer(i8 Num)
{
	LocalPlayers.push_back(Num);
}

bool kra::net::UDPSocket::IsLocalPlayer(i8 Num)
{
	for (auto& It : LocalPlayers)
	{
		if (It == Num)
			return true;
	}
	return false;
}

bool kra::net::UDPSocket::Receive(Packet & Pack, sf::IpAddress & Ip, u16 & Port, KraNetHeader & Header)
{
	CachedPacket.clear();
	Header = KraNetHeader();
	Pack = net::Packet{};

	if (Socket.receive(CachedPacket, Ip, Port) == sf::Socket::Done)
	{
		if (!Header.Load(CachedPacket))
		{
			// Magic number error
			return Receive(Pack, Ip, Port, Header);
		}
		
		// Ignore packages from me
		if (IsLocalPlayer(Header.FromPlayer))
		{
			// Package received from self
			return Receive(Pack, Ip, Port, Header);
		}

		// Ignore package not to me
		if (!IsLocalPlayer(Header.ToPlayer))
		{
			// Package for other player received by this player
			return Receive(Pack, Ip, Port, Header);
		}

		// Load packet
		Pack.Load(CachedPacket);
		return true;
	}

	// Only actually return false if no packet is found
	return false;
}

bool kra::net::UDPSocket::Receive(Packet & Pack, sf::IpAddress & Ip, u16 & Port)
{
	KraNetHeader Header;
	return Receive(Pack, Ip, Port, Header);
}

bool kra::net::UDPSocket::Receive(Packet & Pack, KraNetHeader & Header)
{
	sf::IpAddress Ip;
	u16 Port;
	return Receive(Pack, Ip, Port, Header);
}

bool kra::net::UDPSocket::Send(const Packet & Pack, i8 ThisPlayer, const sf::IpAddress & Ip, u16 Port, i8 OtherPlayer)
{
	assert(IsLocalPlayer(ThisPlayer));

	KraNetHeader Header;
	Header.MagicNumber = MagicNum;
	Header.FromPlayer = ThisPlayer;
	Header.ToPlayer = OtherPlayer;

	// Make packet
	CachedPacket.clear();
	Header.Save(CachedPacket);
	Pack.Save(CachedPacket);

	return (Socket.send(CachedPacket, Ip, Port) == sf::Socket::Done);
}

bool kra::net::UDPSocket::SendFromAll(const Packet & Pack, const sf::IpAddress & Ip, u16 Port, i8 OtherPlayer)
{
	bool Ret = true;

	for (auto& It : LocalPlayers)
	{
		if (It >= 0)
		{
			if (!Send(Pack, It, Ip, Port, OtherPlayer))
			{
				Ret = false;
			}
		}
	}

	return Ret;
}
