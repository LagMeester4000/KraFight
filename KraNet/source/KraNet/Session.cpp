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
	// If the game has not started yet, stop
	if (!Started)
		return;

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
				auto* Pe = Peers.GetPeer(Header.FromPlayer);
				if (Pe)
				{
					Pe->Connected = true;
					Pe->Ready = Rec.Pack.Connect.IsConnectedToAllPeers;
				}
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

				for (Peer& It : Peers.GetLocalPeers())
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
			auto* Pe = Peers.GetPeer(Header.FromPlayer);
			if (Pe)
				MainSocket->SendFromAll(P, Pe->GetIp(), Pe->GetPort(), Pe->GetPlayerNum());
		}
		else
		{
			ProcessPacket(Header, Pack);
		}
	}
}

void kra::net::Session::SendInput()
{
	auto LocalPeers = Peers.GetLocalPeers();

	for (Peer& It : LocalPeers)
	{
		Packet ToSend;
		It.Inputs.SaveToPacket(ToSend);
		Peers.SendToAll(ToSend, It.GetPlayerNum());
	}
}

void kra::net::Session::UpdateInput(const std::vector<KraNetInput>& Input)
{
	auto LocalPeers = Peers.GetLocalPeers();

	assert(LocalPeers.size() == Input.size());

	// Process input
	for (size_t I = 0; I < LocalPeers.size(); ++I)
	{
		LocalPeers[I].get().Inputs.PushInput(Input[I]);
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
	assert(Callback.UpdateFunc);

	auto Inputs = Peers.MakeInputFrame(Peers.GetLocalFrame(Setts));
	Callback.UpdateFunc(External, Inputs.data(), Inputs.size());
}

void kra::net::Session::ProcessPacket(const KraNetHeader & Header, const Packet & Pack)
{
	switch (Pack.Type)
	{
	case PacketType::GameInput:
	{
		// Put game input into correct peer
		Peer* Local = Peers.GetPeer(Header.FromPlayer);
		if (Local && !Local->IsLocal())
		{
			Local->Inputs.LoadFromPacket(Pack);
		}
	} break;
	case PacketType::MissedGameInput:
	{
		// Send back missing input
		Peer* Local = Peers.GetPeer(Header.ToPlayer);
		if (Local && Local->IsLocal())
		{
			Packet MissPack;
			MissPack.Type = PacketType::GameInput;
			Local->Inputs.SaveOldInputToPacket(MissPack, Pack.Pack.MissedGameInput.Frame);
			if (Peer* OtherPeer = Peers.GetPeer(Header.FromPlayer))
				MainSocket->Send(MissPack, Header.ToPlayer, OtherPeer->GetIp(), 
					OtherPeer->GetPort(), OtherPeer->GetPlayerNum());
		}
	} break;
	case PacketType::PingRequest:
	{
		// Return a ping response
		Peer* ReturnPeer = Peers.GetPeer(Header.FromPlayer);
		if (ReturnPeer)
		{
			Packet PingPack;
			PingPack.Type = PacketType::PingResponse;
			PingPack.Pack.PingResponse.TimeAsMs = Pack.Pack.PingRequest.TimeAsMs;
			PingPack.Pack.PingResponse.Frame = Peers.GetLocalFrame(Setts);
			MainSocket->Send(PingPack, Header.FromPlayer, ReturnPeer->GetIp(),
				ReturnPeer->GetPort(), ReturnPeer->GetPlayerNum());
		}
	} break;
	case PacketType::PingResponse:
	{
		// Register the ping along with frame difference
		Peer* OtherPeer = Peers.GetPeer(Header.FromPlayer);
		if (OtherPeer)
		{
			PingTest Ping;
			Ping.Ping = PingClock.getElapsedTime().asMilliseconds() - Pack.Pack.PingResponse.TimeAsMs;
			Ping.LocalFrameDiff = (Peers.GetLocalFrame(Setts) - Ping.Ping / 2) - Pack.Pack.PingResponse.Frame;
			OtherPeer->PingOverTime.PushBack(Ping);
		}
	} break;
	default:
		break;
	}
}

i32 kra::net::Session::GetCurrentFrame()
{
	return UpdateFrame ? Peers.GetLocalFrame(Setts) - 1 : Peers.GetLocalFrame(Setts);
}
