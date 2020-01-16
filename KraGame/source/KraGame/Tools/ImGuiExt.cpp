#include "KraGame/Tools/ImGuiExt.h"
#include <imgui.h>

void game::RenderDragFixedPoint(const char* Label, kra::kfloat& FixedP)
{
	float TestFix = kra::toFloat<float>(FixedP);
	if (ImGui::DragFloat(Label, &TestFix, 0.05f))
		kra::floatToFixed(FixedP, TestFix);
}