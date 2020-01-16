#pragma once
#include <memory>

namespace sf {
	class RenderWindow;
}

namespace game {
	class ResourceManager;

	// Tools
	class AttackBuilder;

	// Class that diplays and updates other tools
	class Toolbar {
	public:
		Toolbar();
		~Toolbar();

		void Update(float DeltaTime, ResourceManager& Resources);
		void Render(sf::RenderWindow& Window, ResourceManager& Resources);

	private:
		bool AttackToolOpen = false;
		std::unique_ptr<AttackBuilder> AttackTool;
	};
}