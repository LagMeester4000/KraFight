#include "KraNet/PeerManager.h"

using namespace kra::net;

Peer & kra::net::PeerManager::GetLocalPeer()
{
	for (auto& It : Peers)
	{
		if (It.IsLocal())
		{
			return It;
		}
	}
	throw;
}

kra::net::PeerManager::PeerManager(std::shared_ptr<UDPSocket> Socket, u32 PlayerAmount, bool Spectator)
{
}

void kra::net::PeerManager::ProcessPacket(i8 From, const Packet & Pack)
{
}

void kra::net::PeerManager::SendToAll(const Packet & Pack)
{
}

void kra::net::PeerManager::AddPeer(i8 PlayerNum, const sf::IpAddress & Ip, u16 Port)
{
}

void kra::net::PeerManager::AddLocalPeer(u16 Port, i8 PlayerNum)
{
}

std::vector<Peer>& kra::net::PeerManager::GetPeers()
{
	// TODO: insert return statement here
}

Peer & kra::net::PeerManager::GetPeer(i8 PlayerNum)
{
	// TODO: insert return statement here
}
