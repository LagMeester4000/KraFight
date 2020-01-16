#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <vector>

namespace game {
	// Aseprite animation loaded via json
	// Does not load the timing info or other unneeded info
	class AnimationResource {
	public:
		// Load the animation into this object
		bool Load(std::string Filename);

		// Getters
		const sf::IntRect& GetFrame(size_t Frame) const;
		const sf::Texture& GetTexture() const;
		size_t GetFramesAmount() const;
		int GetTotalWidth() const;
		int GetTotalHeight() const;
		bool IsLoaded() const;

	private:
		std::vector<sf::IntRect> Frames;
		sf::Texture Texture;
		int Width, Height;
		bool Loaded = false;
	};
}