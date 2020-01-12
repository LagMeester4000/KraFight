#pragma once
#include <KraFight/Hook/IHook.h>

namespace game {
	class ResourceManager;
	class AnimationResource;

	// Hook class that handles events from KraFight
	class Hook : public kra::IHook {
	public: // Interface functions
		virtual void OnEntityCreate(const kra::Context& Con, kra::Handle<kra::Entity> Ent) = 0;
		virtual void OnEntityDestroy(const kra::Context& Con, kra::Handle<kra::Entity> Ent) = 0;
		virtual void OnCustomEvent(const kra::Context& Con, kra::AnyPtr& Event) = 0;

	public:
		// Resource manager
		// Is not owned by this object
		ResourceManager* Resources;
	};
}