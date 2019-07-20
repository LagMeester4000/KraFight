#pragma once
#include "KraFight/Game.h"

namespace sf {
	class RenderWindow;
}

namespace kra {
	class GameRenderer {
	public:
		GameRenderer();

		// Update the game
		void Update(float DeltaTime);

		// Render the game
		void Render(sf::RenderWindow& Window);

	private:
		Game KraGame;
	};
}