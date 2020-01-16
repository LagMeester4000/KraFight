#include "KraGame/Graphics/AttackEvents/AttackEventSetAnimation.h"

using namespace game;

void game::AttackEventSetAnimation::RegisterToAttack(size_t Index, kra::Attack & At)
{
}

const char * game::AttackEventSetAnimation::GetTypeName() const
{
	return "set_animation";
}

IAttackEvent * game::AttackEventSetAnimation::Create() const
{
	return new AttackEventSetAnimation();
}

void game::AttackEventSetAnimation::Load(const nlohmann::json & Json)
{
	std::string AnimFile = Json["animation_file"];
	AnimationFile = AnimFile;
	Offset.X.value = Json["offset_x"];
	Offset.Y.value = Json["offset_y"];
	StartingFrame = Json["starting_frame"];
}

void game::AttackEventSetAnimation::Save(nlohmann::json & Json)
{
	Json["animation_file"] = AnimationFile;
	Json["offset_x"] = Offset.X.value;
	Json["offset_y"] = Offset.Y.value;
	Json["starting_frame"] = StartingFrame;
}

#include <imgui.h>
#include "KraGame/Tools/FileBrowser.h"
#include "KraGame/Tools/ImGuiExt.h"
FileBrowser Browser;
char FileSelector[32] = "";

void game::AttackEventSetAnimation::RenderEditor()
{
	ImGui::InputText("FileName", FileSelector, 32);
	
	ImGui::SameLine();
	if (ImGui::Button("Choose"))
	{
		PressedChoose = true;
		Browser.Open();
		Browser.PermittedExtensions = { ".json" };
	}

	if (PressedChoose)
	{
		ImGui::PushID(this);
		if (Browser.Draw())
		{
			PressedChoose = false;
			if (Browser.Result != "")
			{
				AnimationFile = Browser.Result;
				strcpy(FileSelector, AnimationFile.c_str());
			}
		}
		ImGui::PopID();
	}

	if (ImGui::Button("Update"))
	{
		AnimationFile = FileSelector;
	}

	// Other vars
	ImGui::InputInt("Starting frame", &StartingFrame);
	RenderDragFixedPoint("Offset X", Offset.X);
	RenderDragFixedPoint("Offset Y", Offset.Y);
}
