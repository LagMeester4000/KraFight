#pragma once
#include "FileBrowser.h"
#include <KraFight/TypeDef.h>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace sf {
	class RenderWindow;
}

namespace game {
	class AttackResource;
	class ResourceManager;

	// Class that renders UI to edit an AttackResource
	//   that can also be exported
	class AttackBuilder {
	public:
		AttackBuilder();
		~AttackBuilder();

		void RenderUI(sf::RenderWindow& Window, ResourceManager& Resources);
		void UpdateRenderTexture(ResourceManager& Resources);

	private:
		std::unique_ptr<AttackResource> Attack;
		sf::RenderTexture HitboxDisplayTexture;
		sf::Sprite HitboxDisplaySprite;

		// Time
		kra::kfloat TimeSlider = 0;
		int Timeline;
		bool Playback = false;
		int CurrentSelectedNodeType = 0;
		int SelectedAction = 0;
		char FileSelector[32] = "";

		// File browser
		FileBrowser Files;
		enum class CurrentFileState {
			None,
			SaveAs,
			Load
		} FileState = CurrentFileState::None;
	};
}