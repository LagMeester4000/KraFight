#include "KraGame/Graphics/AttackEvents/AttackEventSetHitbox.h"

using namespace game;

void game::AttackEventSetHitbox::RegisterToAttack(size_t Index, kra::Attack & At)
{
}

const char * game::AttackEventSetHitbox::GetTypeName() const
{
	return "set_hitbox";
}

IAttackEvent * game::AttackEventSetHitbox::Create() const
{
	return new AttackEventSetHitbox();
}

void game::AttackEventSetHitbox::Load(const nlohmann::json & Json)
{
	Index = Json["hitbox_index"];

	const nlohmann::json& Hitb = Json["hitbox"];
	Hit.bActive = Hitb["active"];
	// Position and size
	Hit.Position.X.value = Hitb["pos_x"];
	Hit.Position.Y.value = Hitb["pos_y"];
	Hit.Size.X.value = Hitb["size_x"];
	Hit.Size.Y.value = Hitb["size_y"];

	// HitProperties
	Hit.HitProps.Damage = Hitb["damage"];
	Hit.HitProps.FreezeFrames.value = Hitb["freeze_frames"];
	Hit.HitProps.BlockStun.value = Hitb["block_stun"];
	Hit.HitProps.HitNumber = Hitb["hit_number"];
	Hit.HitProps.Height = Hitb["height"];

	Hit.HitProps.GroundHitstun.value = Hitb["ground_hitstun"];
	Hit.HitProps.GroundKnockback.value = Hitb["ground_knockback"];
	Hit.HitProps.GroundKnockbackOnBlock.value = Hitb["ground_knockback_on_block"];

	Hit.HitProps.AirHitstun.value = Hitb["air_hitstun"];
	Hit.HitProps.bLauncher = Hitb["launcher"];
	Hit.HitProps.LaunchAngle.value = Hitb["launch_angle"];
	Hit.HitProps.LaunchSpeed.value = Hitb["launch_speed"];

	Hit.HitProps.LocalPriority = Hitb["local_priority"];
	Hit.HitProps.GlobalPriority = Hitb["global_priority"];

	Hit.HitProps.bSpecialCancel = Hitb["special_cancel"];
	Hit.HitProps.MeterOnHit.value = Hitb["meter_on_hit"];
	Hit.HitProps.MeterOnBlock.value = Hitb["meter_on_block"];
}

void game::AttackEventSetHitbox::Save(nlohmann::json & Json)
{
	Json["hitbox_index"] = Index;

	nlohmann::json Hitb = nlohmann::json();
	Hitb["active"] = Hit.bActive;
	// Position and size
	Hitb["pos_x"] = Hit.Position.X.value;
	Hitb["pos_y"] = Hit.Position.Y.value;
	Hitb["size_x"] = Hit.Size.X.value;
	Hitb["size_y"] = Hit.Size.Y.value;

	// HitProperties
	Hitb["damage"] = Hit.HitProps.Damage;
	Hitb["freeze_frames"] = Hit.HitProps.FreezeFrames.value;
	Hitb["block_stun"] = Hit.HitProps.BlockStun.value;
	Hitb["hit_number"] = Hit.HitProps.HitNumber;
	Hitb["height"] = Hit.HitProps.Height;

	Hitb["ground_hitstun"] = Hit.HitProps.GroundHitstun.value;
	Hitb["ground_knockback"] = Hit.HitProps.GroundKnockback.value;
	Hitb["ground_knockback_on_block"] = Hit.HitProps.GroundKnockbackOnBlock.value;

	Hitb["air_hitstun"] = Hit.HitProps.AirHitstun.value;
	Hitb["launcher"] = Hit.HitProps.bLauncher;
	Hitb["launch_angle"] = Hit.HitProps.LaunchAngle.value;
	Hitb["launch_speed"] = Hit.HitProps.LaunchSpeed.value;

	Hitb["local_priority"] = Hit.HitProps.LocalPriority;
	Hitb["global_priority"] = Hit.HitProps.GlobalPriority;

	Hitb["special_cancel"] = Hit.HitProps.bSpecialCancel;
	Hitb["meter_on_hit"] = Hit.HitProps.MeterOnHit.value;
	Hitb["meter_on_block"] = Hit.HitProps.MeterOnBlock.value;
	Json["hitbox"] = Hitb;
}

#include <imgui.h>
#include "KraGame/Tools/ImGuiExt.h"
void game::AttackEventSetHitbox::RenderEditor()
{
	ImGui::InputInt("Hitbox index", &Index);
	ImGui::Checkbox("Is active", &Hit.bActive);
	
	ImGui::Separator();
	RenderDragFixedPoint("Position X", Hit.Position.X);
	RenderDragFixedPoint("Position Y", Hit.Position.Y);
	RenderDragFixedPoint("Size X", Hit.Size.X);
	RenderDragFixedPoint("Size Y", Hit.Size.Y);
}
