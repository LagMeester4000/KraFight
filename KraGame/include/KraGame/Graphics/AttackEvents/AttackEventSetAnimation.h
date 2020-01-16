#pragma once
#include "KraGame/Graphics/IAttackEvent.h"
#include <KraFight/Math/Vector2.h>

namespace game {
	class AttackEventSetAnimation : public IAttackEvent {
	public:
		virtual void RegisterToAttack(size_t Index, kra::Attack& At) override;
		virtual const char* GetTypeName() const override;
		virtual IAttackEvent* Create() const override;
		virtual void Load(const nlohmann::json& Json) override;
		virtual void Save(nlohmann::json& Json) override;
		virtual void RenderEditor() override;

	public:
		std::string AnimationFile;
		kra::Vector2 Offset;
		int StartingFrame;

	private: // Editor stuff
		bool PressedChoose = false;
	};
}