#pragma once
#include <KraFight/Detail/Handle.h>
#include <vector>
#include <string>
#include <memory>

namespace sf {
	class Texture;
}

namespace game {
	class AnimationResource;

	class ResourceManager {
	public:
		ResourceManager();

		// Load a texture into the resource manager and get the handle to it
		// If texture is already loaded, just returns the handle
		kra::Handle<sf::Texture> LoadTexture(std::string Filename);

		// Get a texture by a handle
		sf::Texture& GetTexture(kra::Handle<sf::Texture> Hand);

		// Load an animation into the resource manager and get the handle to it
		// If animation is already loaded, just returns the handle
		kra::Handle<AnimationResource> LoadAnimation(std::string Filename);

		// Get an animation by a handle
		AnimationResource& GetAnimation(kra::Handle<AnimationResource> Hand);

	private:
		std::vector<std::unique_ptr<sf::Texture>> Textures;
		std::vector<std::unique_ptr<AnimationResource>> Animations;
	};
}