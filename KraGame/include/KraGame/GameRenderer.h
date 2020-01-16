#pragma once
#include "KraNet/KraNetInput.h"
#include "KraGame/Tools/Toolbar.h"
// I'm so mad that I have to put his include here
// unique_ptr is causing some linking issues
#include <KraFight/Game.h>
#include <memory>

namespace sf {
	class RenderWindow;
}

namespace kra {
	class Game;
	class InputFrame;
	namespace net {
		class KraNetSession;
	}
}

namespace game {
	class ResourceManager;
	class AttackBuilder;

	class GameRenderer {
	public:
		GameRenderer();
		~GameRenderer();

		// Update the game
		void Update(float DeltaTime);

		// Render the game
		void Render(sf::RenderWindow& Window);

		// Render ImGui
		void RenderDebugUI(kra::net::KraNetSession& Ses);

	public:
		static void TryInput(kra::InputFrame& Inp, int Index);
		static void TryInputKeyboard(kra::InputFrame& Inp);

		// Network
		static void NetUpdate(void* Self, KraNetInput P1, KraNetInput P2);
		static void NetSave(void* Self);
		static void NetLoad(void* Self);
		static kra::InputFrame ToKraFightInput(KraNetInput In);
		static KraNetInput ToKraNetInput(kra::InputFrame In);

	private:
		std::unique_ptr<kra::Game> KraFight;
		std::unique_ptr<ResourceManager> Resources;
		Toolbar Tools;
	};
}