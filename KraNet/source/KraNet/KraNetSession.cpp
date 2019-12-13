#include "KraNet/KraNetSession.h"
#include <thread>

kra::net::KraNetSession::KraNetSession(void * ExternalData, void(*UpdateF)(void *, KraNetInput, KraNetInput))
	: External(ExternalData), UpdateFunction(UpdateF), DelayLength(5), Host(false)
{
}

kra::net::KraNetSession::~KraNetSession()
{

}

bool kra::net::KraNetSession::StartConnection(sf::IpAddress OOtherIp, unsigned short PPort)
{
	NetPacket P;
	P.Type = NetPacketType::Setup;
	P.Save(OutPacket);

	if (Sock.send(OutPacket, OOtherIp, PPort) == sf::Socket::Status::Done)
	{
		sf::IpAddress RIp;
		unsigned short RPort;
		if (Sock.receive(InPacket, RIp, RPort) == sf::Socket::Status::Done)
		{
			NetPacket R;
			R.Load(InPacket);

			if (R.Type != NetPacketType::Setup)
				return false;

			Sock.setBlocking(false);
			OtherIp = OOtherIp;
			OtherPort = PPort;
			Initialize();
			Host = false;
			return true;
		}
	}
	
	return false;
}

bool kra::net::KraNetSession::ListenConnection(unsigned short PPort)
{
	if (Sock.bind(PPort) == sf::Socket::Status::Done)
	{
		sf::IpAddress RIp;
		unsigned short RPort;
		if (Sock.receive(InPacket, OtherIp, OtherPort) == sf::Socket::Status::Done)
		{
			NetPacket R;
			R.Load(InPacket);

			if (R.Type != NetPacketType::Setup)
				return false;

			NetPacket P;
			P.Type = NetPacketType::Setup;
			P.Save(OutPacket);
			if (Sock.send(OutPacket, OtherIp, OtherPort) == sf::Socket::Status::Done)
			{
				Sock.setBlocking(false);
				Initialize();
				Host = true;
				return true;
			}
		}
	}
	return false;
}

#include <iostream>
void kra::net::KraNetSession::Update(KraNetInput LocalInput)
{
	ReceiveInput();

	if (Inputs.CanAdvanceGameplayFrame())
	{
		UpdateGameplay();
	}
	else
	{
		std::cout << "Skipped an input frame" << std::endl;
	}

	SendInput();

	if (Inputs.CanAdvanceGameplayFrame())
	{
		UpdateLocalInput(LocalInput);
	}
}

bool kra::net::KraNetSession::IsHost() const
{
	return Host;
}

double kra::net::KraNetSession::GetPing() const
{
	return LastPingTime;
}

sf::IpAddress kra::net::KraNetSession::GetOtherIp() const
{
	return OtherIp;
}

unsigned short kra::net::KraNetSession::GetOtherPort() const
{
	return OtherPort;
}

unsigned short kra::net::KraNetSession::GetDelay() const
{
	return DelayLength;
}

uint32_t kra::net::KraNetSession::GetPingIndex() const
{
	return PingIndex;
}

int64_t kra::net::KraNetSession::GetLastFrameDifference() const
{
	return LastFrameDifference;
}

void kra::net::KraNetSession::Initialize()
{
	Inputs.SetSize(30);
	Inputs.InitializeLocalFrames(DelayLength);
	LastPing = Clock::now();
}

void kra::net::KraNetSession::UpdateGameplay()
{
	auto InputFrame = Inputs.GetGameplayFrame();
	
	if (UpdateFunction)
	{
		if (IsHost())
			UpdateFunction(External, InputFrame.Local, InputFrame.Remote);
		else
			UpdateFunction(External, InputFrame.Remote, InputFrame.Local);
	}
}

void kra::net::KraNetSession::UpdateLocalInput(KraNetInput LocalInput)
{
	Inputs.InsertLocalFrame(DelayLength, LocalInput);
	Inputs.AdvanceGameplayFrame();
}

void kra::net::KraNetSession::SendInput()
{
	// Update ping
	if (PingDone)
	{
		PingDone = false;
		
		auto Now = Clock::now();
		std::chrono::duration<double> Diff = Now - LastPing;
		double PingAsMs = (Diff.count() * 1000.0) / 2.0; // div by 2
		std::cout << "ping: " << PingAsMs << "ms" << std::endl;
		
		// Set ping var
		LastPingTime = PingAsMs;
		
		int64_t LocalFrame = (int64_t)Inputs.GetGameplayFrameIndex(),
			RemoteFrame = (int64_t)InNetPacket.Input.GameplayFrame;
		int64_t PingAsFrames = (int64_t)(PingAsMs / 16.66666666);
		int64_t AdjustedRemoteFrame = RemoteFrame + PingAsFrames;
		int64_t FrameDiff = LocalFrame - AdjustedRemoteFrame;
		std::cout << "Difference in frames: " << FrameDiff << std::endl;
		std::cout << "Local Frame: " << LocalFrame << std::endl;

		// Set frame diff var
		LastFrameDifference = FrameDiff;

		// TEMP 
		// use sleeping to get the two machines synced up
		if (FrameDiff > 0 /*&& IsHost()*/)
		{
			//sleep for half
			std::this_thread::sleep_for(std::chrono::milliseconds(8 * FrameDiff));
			std::cout << "Sleeping for " << 8 * FrameDiff << "ms" << std::endl;
		}

		// Reset the time point
		LastPing = Clock::now();
	}	

	// Saving ping test
	OutNetPacket.Input.PingIndexOut = PingIndex;
	OutNetPacket.Input.PingIndexIn = InNetPacket.Input.PingIndexOut;
	OutNetPacket.Input.GameplayFrame = Inputs.GetGameplayFrameIndex();

	// Saving input
	OutNetPacket.Type = NetPacketType::Input;
	Inputs.MakeSendableInputBuffer(OutNetPacket.Input.Input, OutNetPacket.Input.StartFrame);
	
	// Filling and sending packet
	OutPacket.clear();
	OutNetPacket.Save(OutPacket);
	Sock.send(OutPacket, OtherIp, OtherPort);
}

void kra::net::KraNetSession::ReceiveInput()
{
	sf::IpAddress Ip;
	unsigned short Port;
	/*if (Sock.receive(InPacket, Ip, Port) != sf::Socket::Status::Done)
		return;

	InNetPacket.Load(InPacket);

	// Read inputs
	if (InNetPacket.Type == NetPacketType::Input)
	{
		// Process ping
		if (InNetPacket.Input.PingIndexIn == PingIndex)
		{
			// Setup the boolean to do the actual work in the SendInput function
			PingDone = true;
			PingIndex++;
		}

		// Process inputs
		Inputs.ReadReceivedInputBuffer(InNetPacket.Input.Input, InNetPacket.Input.StartFrame);
	}*/

	while (Sock.receive(InPacket, Ip, Port) == sf::Socket::Status::Done)
	{
		InNetPacket.Load(InPacket);

		// Read inputs
		if (InNetPacket.Type == NetPacketType::Input)
		{
			// Process ping
			if (InNetPacket.Input.PingIndexIn == PingIndex)
			{
				// Setup the boolean to do the actual work in the SendInput function
				PingDone = true;
				PingIndex++;
			}

			// Process inputs
			Inputs.ReadReceivedInputBuffer(InNetPacket.Input.Input, InNetPacket.Input.StartFrame);
		}

		InPacket.clear();
	}
}
