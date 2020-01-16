#pragma once
#include "json/json.hpp"

namespace kra {
	class Attack;
}

namespace game {
	class IAttackEvent {
	public:
		virtual ~IAttackEvent() {}

		// Register this event into an attack object
		virtual void RegisterToAttack(size_t Index, kra::Attack& At) = 0;

		// Get the type name used in json files and such
		virtual const char* GetTypeName() const = 0;

		// Create a new instance of this type
		// This must use the "new" operator
		virtual IAttackEvent* Create() const = 0;

		// Load the object from a json object
		virtual void Load(const nlohmann::json& Json) = 0;

		// Save the object to a json object
		virtual void Save(nlohmann::json& Json) = 0;
		
		// Render the object in ImGui
		virtual void RenderEditor() = 0;
	};
}