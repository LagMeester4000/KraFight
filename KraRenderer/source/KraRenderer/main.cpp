#include <SFML/Graphics.hpp>
#include <iostream>
#include <imgui/imgui-SFML.h>
#include <imgui.h>
#include "KraRenderer/GameRenderer.h"
#include "KraFight/Input/InputFrame.h"
#include "KraNet/KraNetSession.h"

const bool useVsync = false;
const bool useSleep = false;
const float FPS = 60.0f; //The desired FPS. (The number of updates each second).

unsigned short PortNum = 31415;

int main(int argc, char *argv[])
{
	bool redraw = true;      //Do I redraw everything on the screen?

	//actual drawing window
	sf::RenderWindow window(sf::VideoMode(800, 800, 32), "KraRenderer");
	ImGui::SFML::Init(window, true);
	ImGui::CreateContext();
	window.setView(sf::View(sf::FloatRect(-21.6f, -10.f, 31.6f, 20.f)));

	if (useVsync)
		window.setVerticalSyncEnabled(true);

	//game start object
	kra::GameRenderer game;

	//networking
	kra::net::KraNetSession kraNet(&game, &kra::GameRenderer::NetUpdate);
	kraNet.SetRollbackLoadStateFunction(&kra::GameRenderer::NetLoad);
	kraNet.SetRollbackSaveStateFunction(&kra::GameRenderer::NetSave);
	kraNet.SetSimulateFunction(&kra::GameRenderer::NetUpdate);
	{
		std::cout << "0 for host, 1 for join" << std::endl;
		int C;
		std::cin >> C;

		if (C == 0)
		{
			// Host
			//if (!kraNet.ListenConnection(PortNum))
			//{
			//	std::cout << "ERROR: failed to listen for connection" << std::endl;
			//}
			std::cout << "Host code: " << kraNet.StartHost() << std::endl;
		}
		else if (C == 1)
		{
			// Join
			//std::cout << "Ip to connect to:" << std::endl;
			//std::string IpStr;
			//std::cin >> IpStr;
			//sf::IpAddress Ip(IpStr.c_str());
			//if (!kraNet.StartConnection(Ip, PortNum))
			//{
			//	std::cout << "ERROR: failed to join host" << std::endl;
			//}
			std::cout << "Please insert host code: " << std::endl;
			uint32_t Code;
			std::cin >> Code;
			kraNet.StartJoin(Code);
		}
	}

	//clock for timing
	sf::Clock clock;

	sf::Event ev;
	while (window.isOpen())
	{
		auto dtClock = clock.getElapsedTime();
		float deltaTime = dtClock.asSeconds();

		if (useVsync)
			clock.restart();

		//Wait until 1/60th of a second has passed, then update everything.
		if (!useVsync)
		{
			if (deltaTime >= 1.0f / FPS)
			{
				redraw = true; //We're ready to redraw everything
				clock.restart();
			}
			else if (useSleep) //Sleep until next 1/60th of a second comes around
			{
				sf::Time sleepTime = sf::seconds((1.0f / FPS) - clock.getElapsedTime().asSeconds());
				sf::sleep(sleepTime);
			}
		}
		else
		{
			redraw = true;
		}

		//Handle input
		while (window.pollEvent(ev))
		{
			ImGui::SFML::ProcessEvent(ev);

			if (ev.type == sf::Event::Closed) window.close();
		}

		//Draw stuff if ready
		if (redraw)
		{
			redraw = false;
			window.clear(sf::Color(0, 102, 204));

			//draw things here
			//game.Update(1.f / FPS);
			ImGui::SFML::Update(window, dtClock);
			KraNetInput inp;
			if (window.hasFocus())
			{
				kra::InputFrame f;
				kra::GameRenderer::TryInputKeyboard(f);
				inp = kra::GameRenderer::ToKraNetInput(f);
			}
			kraNet.Update(inp);
			game.Render(window);
			game.RenderDebugUI(kraNet);

			ImGui::SFML::Render(window);
			ImGui::EndFrame();
			window.display();
		}
	}

	ImGui::SFML::Shutdown();
	return 0;
}