#include "KraNet/Session.h"
#include "KraNet/Network/UDPSocket.h"
#include <memory>
#include <algorithm>
#include <assert.h>

kra::net::Session::Session(u16 LocalPort, u32 PlayerAmount, bool Spectator)
	: MainSocket(std::make_shared<UDPSocket>()), Peers(MainSocket, PlayerAmount, Spectator)
{
	MainSocket->Init(LocalPort);
}

void kra::net::Session::AddPlayer(i8 PlayerNumber, Address Addr)
{
	if (Started)
	{
		assert(false);
		return;
	}

	Peers.AddPeer(PlayerNumber, Addr.Ip, Addr.port);
}

void kra::net::Session::AddLocalPlayer(i8 PlayerNumber, u16 Port)
{
	if (Started)
	{
		assert(false);
		return;
	}

	Peers.AddLocalPeer(Port, PlayerNumber);
}

void kra::net::Session::AddSpectator(i8 SpectatorNumber, Address Addr)
{
	if (Started)
	{
		assert(false);
		return;
	}

	
}

void kra::net::Session::Start()
{
	assert(!Started);
	Started = true;

	// Call save state function
	if (Callback.SaveState)
		Callback.SaveState(External);
}

void kra::net::Session::Update(const std::vector<KraNetInput>& Input)
{
	if (Peers.ArePeersReady())
	{
		// First receive the input to have an accurate CanUpdate variable
		// Might swap this around
		// Anyway, this function should be called BEFORE rollback so that
		//   networked inputs are also used
		Receive();

		// First check if the client needs to sync up
		// ...
		bool CanUpdate = true;
		UpdateFrame = CanUpdate;

		// Input
		if (CanUpdate)
		{
			UpdateInput(Input);
		}

		// Networking
		SendInput();
		TryRollback();

		// Update Game
		if (CanUpdate)
		{
			UpdateGame();
		}
	}
	else
	{
		// Try to receive connection requests
		KraNetHeader Header;
		Packet Rec;
		while (MainSocket->Receive(Rec, Header))
		{
			if (Rec.Type == PacketType::Connect)
			{
				auto& Peer = Peers.GetPeer(Header.FromPlayer);
				Peer.Connected = true;
				Peer.Ready = Rec.Pack.Connect.IsConnectedToAllPeers;
			}
		}

		// Try to connect to other peers
		for (auto& It : Peers.GetPeers())
		{
			if (!It.IsLocal())
			{
				// Even if they are connected, send the message anyway
				Packet P;
				P.Type = PacketType::Connect;
				P.Pack.Connect.IsConnectedToAllPeers = Peers.ArePeersConnected();

				for (auto& It : Peers.GetLocalPeers())
				{
					MainSocket->SendFromAll(P, It.GetIp(), It.GetPort(), It.GetPlayerNum());
				}
			}
		}
	}
}

void kra::net::Session::SetCallbacks(void * AExternal, Callbacks ACallback)
{
	Callback = ACallback;
	External = AExternal;
}

void kra::net::Session::Receive()
{
	KraNetHeader Header;
	Packet Pack;
	while (MainSocket->Receive(Pack, Header))
	{
		// If a client is still trying to connect, send back the connect confirm 
		//   (only to this peer, because else this would keep happening)
		if (Pack.Type == PacketType::Connect)
		{
			Packet P;
			P.Type = PacketType::Connect;
			P.Pack.Connect.IsConnectedToAllPeers = true;
			auto& Peer = Peers.GetPeer(Header.FromPlayer);
			MainSocket->SendFromAll(P, Peer.GetIp(), Peer.GetPort(), Peer.GetPlayerNum());
		}
		else
		{
			ProcessPacket(Header, Pack);
		}
	}
}

void kra::net::Session::SendInput()
{
	std::vector<Peer&> LocalPeers = Peers.GetLocalPeers();

	for (auto& It : LocalPeers)
	{
		Packet ToSend;
		It.Inputs.SaveToPacket(ToSend);
		Peers.SendToAll(ToSend, It.GetPlayerNum());
	}
}

void kra::net::Session::UpdateInput(const std::vector<KraNetInput>& Input)
{
	std::vector<Peer&> LocalPeers = Peers.GetLocalPeers();

	assert(LocalPeers.size() == Input.size());

	// Process input
	for (size_t I = 0; I < LocalPeers.size(); ++I)
	{
		LocalPeers[I].Inputs.PushInput(Input[I]);
	}
}

void kra::net::Session::TryRollback()
{
	// Some safety checks
	assert(Callback.LoadState);
	assert(Callback.SaveState);
	assert(Callback.SimulateFunc);

	// Current frame to perform rollback upto
	i32 CurrentFrame = GetCurrentFrame();

	// Load the last synced frame
	Callback.LoadState(External);

	// Simulate frames until new confirmed frame
	i32 F = LastConfirmedFrame;
	while (Peers.IsFrameConfirmed(F + 1) && F < CurrentFrame)
	{
		auto Inputs = Peers.MakeInputFrame(F);
		Callback.SimulateFunc(External, Inputs.data(), Inputs.size());
		++F;
	}
	
	// Set new Last confirmed frame and save
	LastConfirmedFrame = F;
	Callback.SaveState(External);

	// Simulate the rest of the frames
	while (F < CurrentFrame)
	{
		auto Inputs = Peers.MakeInputFrame(F);
		Callback.SimulateFunc(External, Inputs.data(), Inputs.size());
		++F;
	}
}

void kra::net::Session::UpdateGame()
{
}

void kra::net::Session::ProcessPacket(const KraNetHeader & Header, const Packet & Pack)
{
}

i32 kra::net::Session::GetCurrentFrame()
{
	return UpdateFrame ? Peers.GetLocalFrame(Setts) - 1 : Peers.GetLocalFrame(Setts);
}
