#pragma once
#include "KraFight/Game.h"
#include "KraNet/KraNetInput.h"

namespace sf {
	class RenderWindow;
}

namespace kra {
	namespace net {
		class KraNetSession;
	}

	class InputFrame;

	class GameRenderer {
	public:
		GameRenderer();

		// Update the game
		void Update(float DeltaTime);

		// Render the game
		void Render(sf::RenderWindow& Window);

		// Render ImGui
		void RenderDebugUI(net::KraNetSession& Ses);

	public:
		static void TryInput(InputFrame& Inp, int Index);
		static void TryInputKeyboard(InputFrame& Inp);

		// Network
		static void NetUpdate(void* Self, KraNetInput P1, KraNetInput P2);
		static void NetSave(void* Self);
		static void NetLoad(void* Self);
		static InputFrame ToKraFightInput(KraNetInput In);
		static KraNetInput ToKraNetInput(InputFrame In);

	private:
		Game KraGame;
	};
}