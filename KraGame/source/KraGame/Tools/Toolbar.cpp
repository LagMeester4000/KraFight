#include "KraGame/Tools/Toolbar.h"
#include "KraGame/Tools/AttackBuilder.h"
#include <imgui.h>

game::Toolbar::Toolbar()
{
	AttackTool = std::make_unique<AttackBuilder>();
}

game::Toolbar::~Toolbar()
{
}

void game::Toolbar::Update(float DeltaTime, ResourceManager & Resources)
{
}

void game::Toolbar::Render(sf::RenderWindow & Window, ResourceManager & Resources)
{
	if (AttackToolOpen)
		AttackTool->RenderUI(Window, Resources);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Attack editor", 0, AttackToolOpen))
				AttackToolOpen = !AttackToolOpen;


			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
