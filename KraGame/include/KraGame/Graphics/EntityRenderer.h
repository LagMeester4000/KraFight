#pragma once
#include <KraFight/Detail/Handle.h>
#include <KraFight/Math/Vector2.h>
#include <SFML/Graphics/Sprite.hpp>

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
	class AnimationResource;

	// Renders a single sprite an entity
	class EntityRenderer {
	public:
		EntityRenderer();

		// Render the sprite
		void Render(const kra::Context& Con, kra::Handle<kra::Entity> Ent, 
			sf::RenderWindow& Window, ResourceManager& Resources);

		// Set if this entity is enabled/exists
		// Should be set by the game systems, not by gameplay code
		void SetEnabled(bool B);

		// Set the animation and load it into the sprite
		void SetAnimation(kra::Handle<AnimationResource> NewAnim, ResourceManager& Resources);

		// Set the animation frame and load it into the sprite
		void SetAnimationFrame(int NewAnim, ResourceManager& Resources);

	public: // Net
		void NetSave(kra::NetSaveBuffer& Buff);
		void NetLoad(kra::NetLoadBuffer& Buff, ResourceManager& Resources);

	private:
		sf::Sprite Sprite;
		bool Enabled = true;
		bool Flipped = false;
		kra::Vector2 Offset;

		// Only used to set the sprite when reloaded
		kra::Handle<AnimationResource> Anim;
		int AnimFrame = 0;
	};
}