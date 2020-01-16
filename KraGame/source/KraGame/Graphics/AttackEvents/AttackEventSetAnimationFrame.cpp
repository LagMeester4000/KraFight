#include "KraGame/Graphics/AttackEvents/AttackEventSetAnimationFrame.h"

using namespace game;

void game::AttackEventSetAnimationFrame::RegisterToAttack(size_t Index, kra::Attack & At)
{
}

const char * game::AttackEventSetAnimationFrame::GetTypeName() const
{
	return "set_animation_frame";
}

IAttackEvent * game::AttackEventSetAnimationFrame::Create() const
{
	return new AttackEventSetAnimationFrame();
}

void game::AttackEventSetAnimationFrame::Load(const nlohmann::json & Json)
{
	Frame = Json["frame"];
}

void game::AttackEventSetAnimationFrame::Save(nlohmann::json & Json)
{
	Json["frame"] = Frame;
}

#include <imgui.h>
void game::AttackEventSetAnimationFrame::RenderEditor()
{
	ImGui::InputInt("Animation Frame", &Frame);
}
