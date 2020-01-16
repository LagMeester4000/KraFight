#include "KraGame/GameRenderer.h"
#include "KraGame/Graphics/ResourceManager.h"
#include "KraGame/Tools/AttackBuilder.h"
#include "KraGame/Graphics/AttackResource.h"
#include "KraGame/Graphics/AttackEvents/AttackEventSetAnimation.h"
#include "KraGame/Graphics/AttackEvents/AttackEventSetAnimationFrame.h"
#include "KraGame/Graphics/AttackEvents/AttackEventSetHitbox.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <imgui.h>
#include <KraFight/Input/InputFrame.h>
#include <KraFight/Physics/PhysicsManager.h>
#include <KraFight/Physics/PhysicsObject.h>
#include <KraFight/Consts.h>
#include <KraFight/Entity/Gameplay/PlayerCharacter.h>
#include <KraFight/Entity/EntityManager.h>
#include <KraFight/Entity/Gameplay/Essentials.h>

using namespace game;

game::GameRenderer::GameRenderer()
{
	KraFight = std::make_unique<kra::Game>();
	Resources = std::make_unique<ResourceManager>();
	auto Point = kra::MakePointer<kra::PlayerCharacter>();
	auto Point2 = kra::MakePointer<kra::PlayerCharacter>();
	KraFight->SetupPlayers(Point, Point2);


	// Setup attack types
	AttackResource::RegisterType<AttackEventSetHitbox>();
	AttackResource::RegisterType<AttackEventSetAnimation>();
	AttackResource::RegisterType<AttackEventSetAnimationFrame>();
}

game::GameRenderer::~GameRenderer()
{
}

void game::GameRenderer::Update(float DeltaTime)
{
	kra::InputFrame P1, P2;

	TryInput(P1, 0);
	TryInputKeyboard(P2);

	KraFight->Update(kra::FrameTime, P1, P2);

	// Tools update
	Tools.Update(DeltaTime, *Resources);
}

void game::GameRenderer::Render(sf::RenderWindow & Window)
{
	Tools.Render(Window, *Resources);

	// Set view
	sf::View v({ 0.f, 0.f }, { 2000.f, 2000.f });
	Window.setView(v);

	auto Con = KraFight->MakeContext();

	// Draw Physics
	for (auto& It : Con.PhysicsObjects->Container())
	{
		if (!It.Exists)
		{
			continue;
		}

		sf::RectangleShape Phys;
		auto Pos = It.Value.GetPosition();
		Phys.setPosition(kra::toFloat<float>(Pos.X), -kra::toFloat<float>(Pos.Y));
		auto Size = It.Value.GetSize();
		sf::Vector2f SfSize = { kra::toFloat<float>(Size.X), kra::toFloat<float>(Size.Y) };
		Phys.setSize(SfSize);
		Phys.setOrigin(SfSize / 2.f);
		Phys.setFillColor(sf::Color::Green);

		// DEBUG
		//std::cout << toFloat<float>(Pos.X) << std::endl;

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
			Box.setPosition(kra::toFloat<float>(Pos.X), -kra::toFloat<float>(Pos.Y));
			auto Size = B.Size;
			sf::Vector2f SfSize = { kra::toFloat<float>(Size.X), kra::toFloat<float>(Size.Y) };
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
			Box.setPosition(kra::toFloat<float>(Pos.X), -kra::toFloat<float>(Pos.Y));
			auto Size = B.Size;
			sf::Vector2f SfSize = { kra::toFloat<float>(Size.X), kra::toFloat<float>(Size.Y) };
			Box.setSize(SfSize);
			Box.setOrigin(SfSize / 2.f);
			Box.setFillColor(sf::Color::Red);

			Window.draw(Box);
		}
	}
}

#include <KraNet/KraNetSession.h>
float PingArray[120];
size_t PingArrayIndex = 0;
float DiffArray[120];
size_t DiffArrayIndex = 0;

void game::GameRenderer::RenderDebugUI(kra::net::KraNetSession& Ses)
{
	ImGui::Begin("Debug");

	PingArray[PingArrayIndex] = (float)Ses.GetPing();
	PingArrayIndex++;
	if (PingArrayIndex >= 120)
		PingArrayIndex = 0;

	ImGui::PlotLines("Ping Over Time", PingArray, 120, 0, 0, 0.f, 200.f, ImVec2(400, 200));

	DiffArray[DiffArrayIndex] = (float)Ses.GetLastFrameDifference();
	DiffArrayIndex++;
	if (DiffArrayIndex >= 120)
		DiffArrayIndex = 0;

	ImGui::PlotLines("Delay Difference", DiffArray, 120, 0, 0, -20.f, 20.f, ImVec2(400, 200));

	ImGui::End();
}

void game::GameRenderer::TryInput(kra::InputFrame & Inp, int Index)
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

void game::GameRenderer::TryInputKeyboard(kra::InputFrame & Inp)
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

void game::GameRenderer::NetUpdate(void * Self, KraNetInput P1, KraNetInput P2)
{
	GameRenderer* R = (GameRenderer*)Self;
	auto P1G = ToKraFightInput(P1);
	auto P2G = ToKraFightInput(P2);
	//R->KraFight->Update(kra::FrameTime, P1G, P2G);
}

void game::GameRenderer::NetSave(void * Self)
{
	GameRenderer* R = (GameRenderer*)Self;
	R->KraFight->StoreState();
}

void game::GameRenderer::NetLoad(void * Self)
{
	GameRenderer* R = (GameRenderer*)Self;
	R->KraFight->RestoreState();
}

kra::InputFrame game::GameRenderer::ToKraFightInput(KraNetInput In)
{
	kra::InputFrame Ret;

	Ret.StickX = In.i0 & 1;
	Ret.StickXNotNull = In.i0 & 2;
	Ret.StickY = In.i0 & 3;
	Ret.StickYNotNull = In.i0 & 4;

	Ret.Attack1.Held = In.i0 & 5;
	Ret.Attack2.Held = In.i0 & 6;
	Ret.Attack3.Held = In.i0 & 7;

	return Ret;
}

KraNetInput game::GameRenderer::ToKraNetInput(kra::InputFrame In)
{
	KraNetInput Ret;

	if (In.StickX)
		Ret.i0 |= 1;
	if (In.StickXNotNull)
		Ret.i0 |= 2;
	if (In.StickY)
		Ret.i0 |= 4;
	if (In.StickYNotNull)
		Ret.i0 |= 8;

	if (In.Attack1.Held)
		Ret.i0 |= 16;
	if (In.Attack2.Held)
		Ret.i0 |= 32;
	if (In.Attack3.Held)
		Ret.i0 |= 64;

	return Ret;
}
