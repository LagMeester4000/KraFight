#include "KraNet/PeerManager.h"
#include "KraNet/Settings.h"

using namespace kra::net;

Peer * kra::net::PeerManager::GetLocalPeer()
{
	for (auto& It : Peers)
	{
		if (It.IsLocal())
		{
			return &It;
		}
	}
	return nullptr;
}

std::vector<std::reference_wrapper<Peer>>  kra::net::PeerManager::GetLocalPeers()
{
	std::vector<std::reference_wrapper<Peer>> LocalPeers;
	for (auto& It : GetPeers())
	{
		if (It.IsLocal())
		{
			LocalPeers.push_back(It);
		}
	}
	std::sort(LocalPeers.begin(), LocalPeers.end(), [](Peer& f1, Peer& f2)
	{
		return f1.GetPlayerNum() > f2.GetPlayerNum();
	});
	return LocalPeers;
}

bool kra::net::PeerManager::ArePeersReady() const
{
	return false;
}

bool kra::net::PeerManager::ArePeersConnected() const
{
	return false;
}

i32 kra::net::PeerManager::GetLocalFrame(const Settings & Sett)
{
	return GetLastLocalInputFrame() - Sett.InputDelay;
}

i32 kra::net::PeerManager::GetLastLocalInputFrame()
{
	Peer* P = GetLocalPeer();
	assert(P);
	return P->Inputs.GetFrame();
}

std::vector<KraNetInput> kra::net::PeerManager::MakeInputFrame(i32 Frame)
{
	return std::vector<KraNetInput>();
}

bool kra::net::PeerManager::IsFrameConfirmed(i32 Frame)
{
	return false;
}

kra::net::PeerManager::PeerManager(std::shared_ptr<UDPSocket> ASocket, u32 APlayerAmount, bool ASpectator)
	: Socket(ASocket), AmountOfPlayers(APlayerAmount), Spectator(ASpectator)
{

}

//void kra::net::PeerManager::ProcessPacket(const KraNetHeader & Header, const Packet & Pack)
//{
//
//}

void kra::net::PeerManager::SendToAll(const Packet & Pack, i8 FromPlayerNum)
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
	return Peers;
}

Peer * kra::net::PeerManager::GetPeer(i8 PlayerNum)
{
	for (auto& It : Peers)
	{
		if (It.GetPlayerNum() == PlayerNum)
		{
			return &It;
		}
	}
	return nullptr;
}
