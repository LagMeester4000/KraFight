#include "KraNet/Packet.h"
#include <SFML/Network/Packet.hpp>

void kra::net::Packet::Load(sf::Packet & APack)
{
	APack >> Type;
	switch (Type)
	{
	case PacketType::Connect:
	{
		APack >> Pack.Connect.IsConnectedToAllPeers;
	} break;
	case PacketType::GameInput:
	{
		APack >> Pack.GameInput.Frame;
		APack >> Pack.GameInput.InputSize;
		for (u32 I = 0; I < Pack.GameInput.InputSize; ++I)
		{
			auto& It = Pack.GameInput.Inputs[I];
			APack >> It.i0;
			APack >> It.i1;
			APack >> It.i2;
			APack >> It.i3;
			APack >> Pack.GameInput.InputsValid[I];
		}
	} break;
	case PacketType::MissedGameInput:
	{
		APack >> Pack.MissedGameInput.Frame;
	} break;
	case PacketType::PingRequest:
	{
		APack >> Pack.PingRequest.TimeAsMs;
	} break;
	case PacketType::PingResponse:
	{
		APack >> Pack.PingResponse.TimeAsMs;
		APack >> Pack.PingResponse.Frame;
	} break;
	default:
		break;
	}
}

void kra::net::Packet::Save(sf::Packet & APack) const
{
	APack << Type;
	switch (Type)
	{
	case PacketType::Connect:
	{
		APack << Pack.Connect.IsConnectedToAllPeers;
	} break;
	case PacketType::GameInput:
	{
		APack << Pack.GameInput.Frame;
		APack << Pack.GameInput.InputSize;
		for (u32 I = 0; I < Pack.GameInput.InputSize; ++I)
		{
			auto& It = Pack.GameInput.Inputs[I];
			APack << It.i0;
			APack << It.i1;
			APack << It.i2;
			APack << It.i3;
			APack << Pack.GameInput.InputsValid[I];
		}
	} break;
	case PacketType::MissedGameInput:
	{
		APack << Pack.MissedGameInput.Frame;
	} break;
	case PacketType::PingRequest:
	{
		APack << Pack.PingRequest.TimeAsMs;
	} break;
	case PacketType::PingResponse:
	{
		APack << Pack.PingResponse.TimeAsMs;
		APack << Pack.PingResponse.Frame;
	} break;
	default:
		break;
	}
}
