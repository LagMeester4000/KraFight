#pragma once
#include "EntityRenderer.h"
#include <vector>

namespace sf {
	class RenderWindow;
}

namespace kra {
	class Entity;
	struct Context;
	class NetSaveBuffer;
	class NetLoadBuffer;
}

namespace game {
	class ResourceManager;

	class Renderer {
	public:
		Renderer();

		// Register/add an entity to the renderer
		void Add(kra::Handle<kra::Entity> Ent);

		// Remove an entity from the renderer
		void Remove(kra::Handle<kra::Entity> Ent);

		// Render all the active sprites
		void Render(const kra::Context& Con, sf::RenderWindow& Window, ResourceManager& Resources);

	public: // Net
		// Save the current state
		void NetSave(kra::NetSaveBuffer& Buff);

		// Restore the saved state
		void NetLoad(kra::NetLoadBuffer& Buff, ResourceManager& Resources);

	private:
		std::vector<EntityRenderer> EntityRenderer;
	};
}