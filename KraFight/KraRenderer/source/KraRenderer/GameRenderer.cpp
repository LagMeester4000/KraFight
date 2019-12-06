#include "KraRenderer/GameRenderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "KraFight/Input/InputFrame.h"
#include "KraFight/Physics/PhysicsManager.h"
#include "KraFight/Physics/PhysicsObject.h"
#include "KraFight/Consts.h"
#include "KraFight/Entity/Gameplay/PlayerCharacter.h"
#include "KraFight/Entity/EntityManager.h"
#include "KraFight/Entity/Gameplay/Essentials.h"

kra::GameRenderer::GameRenderer()
{
	auto Point = kra::MakePointer<kra::PlayerCharacter>();
	auto Point2 = kra::MakePointer<kra::PlayerCharacter>();
	KraGame.SetupPlayers(Point, Point2);
}

void kra::GameRenderer::Update(float DeltaTime)
{
	InputFrame P1, P2;

	TryInput(P1, 0);
	TryInputKeyboard(P2);

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
		Phys.setPosition(toFloat<float>(Pos.X), -toFloat<float>(Pos.Y));
		auto Size = It.Value.GetSize();
		sf::Vector2f SfSize = { toFloat<float>(Size.X), toFloat<float>(Size.Y) };
		Phys.setSize(SfSize);
		Phys.setOrigin(SfSize / 2.f);
		Phys.setFillColor(sf::Color::Green);

		// DEBUG
		std::cout << toFloat<float>(Pos.X) << std::endl;

		Window.draw(Phys);
	}

	// Draw Hurtboxes
	for (auto& It : Con.Hitboxes->HurtContainer())
	{
		if (!It.Exists)
		{
			continue;
		}

		// Find base position
		auto BasePos = Con.PhysicsObjects->Get(Con.Entities->Get(It.Value.GetOwner())->GetPhysicsBody()).GetPosition();

		for (auto& B : It.Value.GetContainer())
		{
			if (!B.bActive)
			{
				continue;
			}

			sf::RectangleShape Box;
			auto Pos = B.Position + BasePos;
			Box.setPosition(toFloat<float>(Pos.X), -toFloat<float>(Pos.Y));
			auto Size = B.Size;
			sf::Vector2f SfSize = { toFloat<float>(Size.X), toFloat<float>(Size.Y) };
			Box.setSize(SfSize);
			Box.setOrigin(SfSize / 2.f);
			Box.setFillColor(sf::Color::Blue);

			Window.draw(Box);
		}
	}

	// Draw Hitboxes
	for (auto& It : Con.Hitboxes->HitContainer())
	{
		if (!It.Exists)
		{
			continue;
		}

		// Find base position
		auto BasePos = Con.PhysicsObjects->Get(Con.Entities->Get(It.Value.GetOwner())->GetPhysicsBody()).GetPosition();

		for (auto& B : It.Value.GetContainer())
		{
			if (!B.bActive)
			{
				continue;
			}

			sf::RectangleShape Box;
			auto Pos = B.Position + BasePos;
			Box.setPosition(toFloat<float>(Pos.X), -toFloat<float>(Pos.Y));
			auto Size = B.Size;
			sf::Vector2f SfSize = { toFloat<float>(Size.X), toFloat<float>(Size.Y) };
			Box.setSize(SfSize);
			Box.setOrigin(SfSize / 2.f);
			Box.setFillColor(sf::Color::Red);

			Window.draw(Box);
		}
	}
}

void kra::GameRenderer::TryInput(InputFrame & Inp, int Index)
{
	if (sf::Joystick::isConnected(Index))
	{
		float X = sf::Joystick::getAxisPosition(Index, sf::Joystick::X);
		if (X > 50.f || X < -50.f)
		{
			Inp.StickXNotNull = true;
			if (X > 50.f)
			{
				Inp.StickX = true;
			}
		}
		float Y = sf::Joystick::getAxisPosition(Index, sf::Joystick::Y);
		if (Y > 50.f || Y < -50.f)
		{
			Inp.StickYNotNull = true;
			if (Y < -50.f)
			{
				Inp.StickY = true;
			}
		}

		Inp.Attack1.Held = sf::Joystick::isButtonPressed(Index, 1);
		Inp.Attack2.Held = sf::Joystick::isButtonPressed(Index, 2);
	}
}

void kra::GameRenderer::TryInputKeyboard(InputFrame & Inp)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		Inp.StickXNotNull = true;
		Inp.StickX = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		Inp.StickXNotNull = true;
		Inp.StickX = true;
	}
	else
	{
		Inp.StickXNotNull = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		Inp.StickYNotNull = true;
		Inp.StickY = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		Inp.StickYNotNull = true;
		Inp.StickY = true;
	}
	else
	{
		Inp.StickYNotNull = false;
	}

	Inp.Attack1.Held = sf::Keyboard::isKeyPressed(sf::Keyboard::J);
	Inp.Attack2.Held = sf::Keyboard::isKeyPressed(sf::Keyboard::K);
}
