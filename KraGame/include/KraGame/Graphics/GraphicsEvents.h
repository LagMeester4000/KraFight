#pragma once
#include <KraFight/Detail/Handle.h>
#include <KraFight/Math/Vector2.h>

namespace kra {
	class Entity;
}

namespace game {
	class AnimationResource;

	namespace events {
		using EntityHandle = kra::Handle<kra::Entity>;

		struct EventSetSpriteAnimation {
			EntityHandle Ent;
			kra::Handle<AnimationResource> Animation;
		};

		struct EventSetSpriteAnimationFrame {
			EntityHandle Ent;
			int AnimationFrame;
		};

		struct EventSetSpriteFlipped {
			EntityHandle Ent;
			bool Flipped;
		};

		struct EventSetSpriteOffset {
			EntityHandle Ent;
			kra::Vector2 Offset;
		};
	}
}