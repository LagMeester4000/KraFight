#pragma once
#include <memory>

namespace sf {
	class RenderWindow;
}

namespace kra {
	class Game;
}

namespace game {
	class ResourceManager;

	class Application {
	public:
		Application();

		// Update all the game systems
		void Update(float DeltaTime);

		// Render the game
		void Render(sf::RenderWindow& Window);

	private:
		std::unique_ptr<kra::Game> KraFight;
		std::unique_ptr<ResourceManager> Resources;
	};
}