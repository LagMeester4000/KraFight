#pragma once
#include "IAttackEvent.h"
#include "mapbox/variant.hpp"
#include <KraFight/Behavior/Attack.h>
#include <KraFight/Hitbox/Hitbox.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace kra {
	class ResourceManager;
}

namespace game {
	class AttackResource {
	public:
		using AOption = std::unique_ptr<IAttackEvent>;

		struct Members {
			std::string Name;
			int TotalFrames;
			std::map<int, std::vector<AOption>> Frames;
		};

		struct OptionTypes {
			std::map<std::string, AOption> OptionTypes;
		};

	public:
		AttackResource();

		// Register the attack in the KraFight resource manager
		void RegisterAttack(kra::ResourceManager& Resources);

		// Make an Attack object out of the data provided by the resource
		kra::Attack MakeAttack();

		// Saves the resource to a file
		void Save(std::string Filename) const;

		// Loads the resource from a file
		void Load(std::string Filename);

		// Edit the resource
		Members& Edit();

		// Register an option type (globally)
		template<typename T>
		static void RegisterType()
		{
			AOption NewType = AOption(new T());
			std::string NewTypeName = NewType->GetTypeName();
			auto Find = Types.OptionTypes.find(NewTypeName);
			if (Find == Types.OptionTypes.end())
			{
				Types.OptionTypes[NewTypeName] = std::move(NewType);
			}
		}

		static IAttackEvent* GetType(std::string& TypeName)
		{
			auto Find = Types.OptionTypes.find(TypeName);
			if (Find != Types.OptionTypes.end())
			{
				return Find->second.get();
			}
			return nullptr;
		}

	private:
		Members Mems;
		static OptionTypes Types;
	};
}