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
#include "KraFight/Entity/Gameplay/Essentials.h"

kra::GameRenderer::GameRenderer()
{
	auto Con = KraGame.MakeContext();

	auto SM1Point = kra::MakePointer<PlayerStateMachine>(EPlayerStates::Idle);
	auto SM1 = Con.StateMachines->Add(SM1Point);
	auto SM2Point = kra::MakePointer<PlayerStateMachine>(EPlayerStates::Idle);
	auto SM2 = Con.StateMachines->Add(SM2Point);

	auto Point = kra::MakePointer<kra::PlayerCharacter>();
	Point->SetupPlayer(kra::Handle<kra::InputBuffer>(0), SM1, 0);
	auto Point2 = kra::MakePointer<kra::PlayerCharacter>();
	Point2->SetupPlayer(kra::Handle<kra::InputBuffer>(1), SM2, 1);

	auto P1 = Con.Entities->Add(Point, Con);
	auto P2 = Con.Entities->Add(Point2, Con);

	SM1Point->SetOwner(P1);
	SM2Point->SetOwner(P2);

	auto SMS1 = kra::PlayerStateMachineSetup(*SM1Point);
	Point->SetupStateMachine(SMS1);
	auto SMS2 = kra::PlayerStateMachineSetup(*SM2Point);
	Point2->SetupStateMachine(SMS2);
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
		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		if (Y > 50.f || Y < -50.f)
		{
			P1.StickYNotNull = true;
			if (Y < -50.f)
			{
				P1.StickY = true;
			}
		}

		P1.Attack1.Held = sf::Joystick::isButtonPressed(0, 1);
		P1.Attack2.Held = sf::Joystick::isButtonPressed(0, 2);
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
