#include "KraNet/Network/UDPSocket.h"

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

	// Set magic number
	LocalHeader.MagicNumber = MagicNum;
}

void kra::net::UDPSocket::SetPlayerNumber(i8 Num)
{
	LocalHeader.FromPlayer = Num;
}

bool kra::net::UDPSocket::Receive(Packet & Pack, sf::IpAddress & Ip, u16 & Port, KraNetHeader & Header)
{
	CachedPacket.clear();
	if (Socket.receive() == sf::Socket::Done)
	{
		Header = KraNetHeader();


	}

	return false;
}

bool kra::net::UDPSocket::Receive(Packet & Pack, sf::IpAddress & Ip, u16 & Port)
{
	return false;
}

bool kra::net::UDPSocket::Send(const Packet & Pack, const sf::IpAddress & Ip, u16 Port, i8 OtherPlayer)
{
	return false;
}
