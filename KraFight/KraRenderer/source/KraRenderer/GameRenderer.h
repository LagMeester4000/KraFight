#pragma once
#include "KraFight/Game.h"

namespace sf {
	class RenderWindow;
}

namespace kra {
	class InputFrame;

	class GameRenderer {
	public:
		GameRenderer();

		// Update the game
		void Update(float DeltaTime);

		// Render the game
		void Render(sf::RenderWindow& Window);

	private:
		void TryInput(InputFrame& Inp, int Index);
		void TryInputKeyboard(InputFrame& Inp);

	private:
		Game KraGame;
	};
}