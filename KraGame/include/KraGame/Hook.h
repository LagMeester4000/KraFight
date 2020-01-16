#pragma once
#include <KraFight/Hook/IHook.h>

namespace game {
	class ResourceManager;
	class AnimationResource;
	class Renderer;

	// Hook class that handles events from KraFight
	class Hook : public kra::IHook {
	public: // Interface functions
		virtual void OnEntityCreate(const kra::Context& Con, kra::Handle<kra::Entity> Ent);
		virtual void OnEntityDestroy(const kra::Context& Con, kra::Handle<kra::Entity> Ent);
		virtual void OnCustomEvent(const kra::Context& Con, kra::AnyPtr& Event);

	public:
		// Resource manager
		// Is not owned by this object
		ResourceManager* Resources;

		// Renderer
		// Is not owned by this object
		Renderer* Renderer;
	};
}