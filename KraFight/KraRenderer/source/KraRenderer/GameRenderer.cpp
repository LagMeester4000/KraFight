#include "GameRenderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "KraFight/Input/InputFrame.h"
#include "KraFight/Physics/PhysicsManager.h"
#include "KraFight/Physics/PhysicsObject.h"
#include "KraFight/Consts.h"
#include "KraFight/Entity/Gameplay/PlayerCharacter.h"
#include "KraFight/Entity/EntityManager.h"

kra::GameRenderer::GameRenderer()
{
	auto Point = kra::MakePointer<kra::PlayerCharacter>();
	Point->SetupPlayer(kra::Handle<kra::InputBuffer>(0), 0);
	auto Con = KraGame.MakeContext();
	Con.Entities->Add(Point, Con);
}

void kra::GameRenderer::Update(float DeltaTime)
{
	InputFrame P1, P2;

	if (sf::Joystick::isConnected(0))
	{
		float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		if (X > 50.f || X < -50.f)
		{
			P1.StickXNotNull = true;
			if (X > 50.f)
			{
				P1.StickX = true;
			}
		}
	}

	KraGame.Update(kra::FrameTime, P1, P2);
}

void kra::GameRenderer::Render(sf::RenderWindow & Window)
{
	// Set view
	sf::View v({ 0.f, 0.f }, { 2000.f, 2000.f });
	Window.setView(v);

	auto Con = KraGame.MakeContext();

	// Draw Physics
	for (auto& It : Con.PhysicsObjects->Container())
	{
		if (!It.Exists)
		{
			continue;
		}

		sf::RectangleShape Phys;
		auto Pos = It.Value.GetPosition();
		Phys.setPosition(Pos.X.toFloat(), Pos.Y.toFloat());
		auto Size = It.Value.GetSize();
		sf::Vector2f SfSize = { Size.X.toFloat(), Size.Y.toFloat() };
		Phys.setSize(SfSize);
		Phys.setOrigin(SfSize / 2.f);
		Phys.setFillColor(sf::Color::Green);

		// DEBUG
		std::cout << Pos.X.toFloat() << std::endl;

		Window.draw(Phys);
	}


}
