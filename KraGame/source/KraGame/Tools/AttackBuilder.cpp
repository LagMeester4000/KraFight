#include "KraGame/Tools/AttackBuilder.h"
#include "KraGame/Graphics/AttackResource.h"
#include "KraGame/Graphics/ResourceManager.h"
#include "KraGame/Graphics/AnimationResource.h"
#include "KraGame/Graphics/AttackEvents/AttackEventSetAnimation.h"
#include "KraGame/Graphics/AttackEvents/AttackEventSetAnimationFrame.h"
#include "KraGame/Graphics/AttackEvents/AttackEventSetHitbox.h"
#include "KraGame/Consts.h"
#include <KraFight/Consts.h>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui/imgui-SFML.h>
#include <exception>
#include <iostream>

game::AttackBuilder::AttackBuilder()
{
	Attack = std::make_unique<AttackResource>();
	HitboxDisplayTexture.create(800, 800);
	sf::View View(sf::FloatRect(-128.f, 128.f, 256.f, -256.f));
	HitboxDisplayTexture.setView(View);

	TimeSlider = 0;
	Timeline = 0;
	Attack->Edit().TotalFrames = 0;
}

game::AttackBuilder::~AttackBuilder()
{

}

const char* NodeTypes[] = {
	"Set Hitbox",
	"Set Animation",
	"Set Animation Frame"
};

void game::AttackBuilder::RenderUI(sf::RenderWindow & Window, game::ResourceManager& Resources)
{
	//TEMP
	//Files.Draw();
	//ImGui::ShowDemoWindow();

	if (ImGui::Begin("Hitbox Editor", 0, ImGuiWindowFlags_MenuBar))
	{
		// Render menu bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					FileState = CurrentFileState::SaveAs;
					Files.PermittedExtensions = { ".json" };
					Files.Open();
					Files.Save = true;
				}
				if (ImGui::MenuItem("Save as"))
				{
					FileState = CurrentFileState::SaveAs;
					Files.PermittedExtensions = { ".json" };
					Files.Open();
					Files.Save = true;
				}
				if (ImGui::MenuItem("Load"))
				{
					FileState = CurrentFileState::Load;
					Files.PermittedExtensions = { ".json" };
					Files.Open();
					Files.Save = false;
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// Handle saving and loading of attack
		if (FileState != CurrentFileState::None)
		{
			if (FileState == CurrentFileState::SaveAs)
			{
				if (Files.Draw())
				{
					if (Files.Result != "")
					{
						Attack->Save(Files.Result);
					}
					FileState = CurrentFileState::None;
				}
			}
			else if (FileState == CurrentFileState::Load)
			{
				if (Files.Draw())
				{
					if (Files.Result != "")
					{
						try 
						{
							Attack->Load(Files.Result);
						}
						catch (const std::exception&)
						{
							std::cout << "Failed to load attack" << std::endl;
						}
					}
					FileState = CurrentFileState::None;
				}
			}
		}

		// Render display
		UpdateRenderTexture(Resources);
		ImGui::Image(HitboxDisplayTexture.getTexture());


		// Render editing UI
		auto& Edit = Attack->Edit();

		if (Timeline >= Edit.TotalFrames && Edit.TotalFrames > 0)
		{
			Timeline = Edit.TotalFrames;
		}
		
		// Timeline
		ImGui::InputInt("Length in frames", &Edit.TotalFrames);
		ImGui::SliderInt("Timeline", &Timeline, 0, Edit.TotalFrames > 0 ? Edit.TotalFrames - 1 : Edit.TotalFrames);
		ImGui::Checkbox("Play", &Playback);
		if (Playback)
		{
			//TimeSlider += kra::FrameTime;
			// For now just do 1 frame per frame
			Timeline++;
			if (Timeline > Edit.TotalFrames)
				Timeline = 0;
		}


		// Actions
		ImGui::Separator();
		ImGui::Combo("Type", &CurrentSelectedNodeType, NodeTypes, 3);

		if (ImGui::Button("Add"))
		{
			auto& Find = Edit.Frames[Timeline];
			if (CurrentSelectedNodeType == 0)
			{
				// Set Hitbox
				Find.push_back(
					std::unique_ptr<IAttackEvent>(new AttackEventSetHitbox())
				);
			}
			else if (CurrentSelectedNodeType == 1)
			{
				// Set Animation
				Find.push_back(
					std::unique_ptr<IAttackEvent>(new AttackEventSetAnimation())
				);
			}
			else if (CurrentSelectedNodeType == 2)
			{
				// Set Animation Frame
				Find.push_back(
					std::unique_ptr<IAttackEvent>(new AttackEventSetAnimationFrame())
				);
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			auto Find = Edit.Frames.find(Timeline);
			if (Find != Edit.Frames.end())
			{
				if (SelectedAction < Find->second.size())
				{
					Find->second.erase(Find->second.begin() + SelectedAction);
				}
			}
		}

		ImGui::Separator();

		// List of actions
		auto Find = Edit.Frames.find(Timeline);
		if (Find != Edit.Frames.end())
		{
			// List of actions
			for (size_t I = 0; I < Find->second.size(); ++I)
			{
				char Buf[32];
				auto& It = Find->second[I];
				sprintf(Buf, "%d %s", I, It->GetTypeName());

				if (ImGui::Selectable(Buf, SelectedAction == I))
				{
					SelectedAction = I;
				}
			}

			ImGui::Separator();

			// Show action attributes
			if (SelectedAction < Find->second.size())
			{
				auto& It = Find->second[SelectedAction];
				It->RenderEditor();
			}
		}

	}
	ImGui::End();
}

void game::AttackBuilder::UpdateRenderTexture(ResourceManager & Resources)
{
	struct CHitbox {
		kra::Hitbox Hit;
		bool Active = false;
	};
	auto& Edit = Attack->Edit();

	// Find current parrameters
	int AnimationFrame = 0;
	AnimationResource* Animation = nullptr;
	kra::Vector2 AnimationOffset = { 0, 0 };
	std::vector<CHitbox> Hitboxes(10);
	{
		//int32_t CurrentFrame = (TimeSlider / kra::FrameTime).toInt();
		int32_t CurrentFrame = Timeline;
		for (int32_t I = 0; I <= CurrentFrame; ++I)
		{
			auto Find = Edit.Frames.find(I);
			if (Find != Edit.Frames.end())
			{
				for (auto& It : Find->second)
				{
					auto* ItRaw = It.get();
					if (auto* AsSetHitbox = dynamic_cast<AttackEventSetHitbox*>(ItRaw))
					{
						if (AsSetHitbox->Index >= 10)
							return;
						Hitboxes[AsSetHitbox->Index].Active = true;
						Hitboxes[AsSetHitbox->Index].Hit = AsSetHitbox->Hit;

					}
					else if (auto* AsSetAnimation = dynamic_cast<AttackEventSetAnimation*>(ItRaw))
					{
						auto Hand = Resources.LoadAnimation(AsSetAnimation->AnimationFile);
						if (Hand.IsValid())
							Animation = &Resources.GetAnimation(Hand);
						AnimationOffset = AsSetAnimation->Offset;
						AnimationFrame = AsSetAnimation->StartingFrame;
					}
					else if (auto* AsSetAnimationFrame = dynamic_cast<AttackEventSetAnimationFrame*>(ItRaw))
					{
						AnimationFrame = AsSetAnimationFrame->Frame;
					}

				}
			}
		}
	}


	// Draw to render texture
	HitboxDisplayTexture.clear();

	// First draw player sprite
	if (Animation)
	{
		HitboxDisplaySprite.setTexture(Animation->GetTexture());
		auto Rect = Animation->GetFrame((size_t)AnimationFrame % Animation->GetFramesAmount());
		HitboxDisplaySprite.setTextureRect(Rect);
		HitboxDisplaySprite.setOrigin(Rect.width / 2.f, Rect.height / 2.f); // maybe uses sfml space and not texture space
		HitboxDisplaySprite.setScale(PixelScale, PixelScale);
		HitboxDisplaySprite.setPosition(kra::toFloat<float>(AnimationOffset.X), 
			kra::toFloat<float>(-AnimationOffset.Y));
		HitboxDisplayTexture.draw(HitboxDisplaySprite);
	}

	// Then draw physics display in blue
	{
		sf::RectangleShape Phys;
		sf::Color InColor(50, 50, 255, 100);
		Phys.setFillColor(InColor);
		Phys.setPosition(0.f, 0.f);
		Phys.setSize({ 50.f, 100.f });
		Phys.setOrigin(25.f, 50.f);
		HitboxDisplayTexture.draw(Phys);
	}

	// Then draw hitboxes
	for (auto& It : Hitboxes)
	{
		if (It.Active && It.Hit.bActive)
		{
			// Render red hitbox
			sf::RectangleShape Box;
			Box.setOutlineThickness(0.02f);
			sf::Color OutlineColor(255, 130, 130, 100);
			sf::Color InColor(255, 0, 0, 100);
			Box.setOutlineColor(OutlineColor);
			Box.setFillColor(InColor);
			sf::Vector2f FloatPos = { kra::toFloat<float>(It.Hit.Position.X), 
				kra::toFloat<float>(-It.Hit.Position.Y) };
			Box.setPosition(FloatPos);
			sf::Vector2f FloatSize = { kra::toFloat<float>(It.Hit.Size.X), 
				kra::toFloat<float>(It.Hit.Size.Y)};
			Box.setSize(FloatSize);
			Box.setOrigin(FloatSize * 0.5f);
			HitboxDisplayTexture.draw(Box);
		}
	}
}
