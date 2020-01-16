#pragma once
#include "KraGame/Graphics/IAttackEvent.h"
#include <KraFight/Hitbox/Hitbox.h>

namespace game {
	class AttackEventSetHitbox : public IAttackEvent {
	public:
		virtual void RegisterToAttack(size_t Index, kra::Attack& At) override;
		virtual const char* GetTypeName() const override;
		virtual IAttackEvent* Create() const override;
		virtual void Load(const nlohmann::json& Json) override;
		virtual void Save(nlohmann::json& Json) override;
		virtual void RenderEditor() override;

	public:
		int Index;
		kra::Hitbox Hit;
	};
}