#include "KraNetwork.h"
#include <iostream>
#include <SFML/Network.hpp>

unsigned short PortNum = 31415;
sf::IpAddress IP = "192.168.2.158";

int main()
{
	int Choose;
	std::cout << "0 for server, 1 for client" << std::endl;
	std::cin >> Choose;

	if (Choose)
	{
		// Client
		sf::UdpSocket Sock;
		//if (Sock.bind(PortNum/*, IP*/) == sf::Socket::Done)
		{
			for (int I = 0; I < 3; ++I)
			{
				sf::Packet Pack;
				Pack << I;
				auto Stat = Sock.send(Pack, IP, PortNum);
			}
		}
		//else
		//{
		//	std::cout << "failed to bind on client" << std::endl;
		//}
		
	}
	else
	{
		// Server
		sf::UdpSocket Sock;
		if (Sock.bind(PortNum/*, IP*/) == sf::Socket::Done)
		{
			Sock.setBlocking(false);

			int Leave = 1;
			std::cout << "1 to continue, 0 to exit" << std::endl;
			std::cin >> Leave;
			while (Leave == 1)
			{
				//sf::Uint32
				sf::Packet Pack;
				sf::IpAddress OtherIP;
				unsigned short OtherPort;
				while (Sock.receive(Pack, OtherIP, OtherPort) == sf::Socket::Done)
				{
					int PrintInt;
					Pack >> PrintInt;
					std::cout << PrintInt << std::endl;

					Pack.clear();
				}

				std::cout << "1 to retry, 0 to exit" << std::endl;
				std::cin >> Leave;
			}
		}
	}

	system("pause");
	return 1;
}