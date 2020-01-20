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
