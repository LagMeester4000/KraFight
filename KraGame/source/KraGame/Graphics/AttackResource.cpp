#include "KraGame/Graphics/AttackResource.h"
#include "json/json.hpp"
#include <KraFight/Resource/ResourceManager.h>
#include <fstream>
#include <iostream>

using namespace game;
using json = nlohmann::json;

AttackResource::OptionTypes AttackResource::Types = AttackResource::OptionTypes();

game::AttackResource::AttackResource()
{
}

void game::AttackResource::RegisterAttack(kra::ResourceManager & Resources)
{
	Resources.Attacks.MakeResource(Mems.Name, std::move(MakeAttack()));
}

kra::Attack game::AttackResource::MakeAttack()
{
	return kra::Attack();
}

void game::AttackResource::Save(std::string Filename) const
{
	json Write = json();
	
	// Write metadata
	Write["meta"]["total_frames"] = Mems.TotalFrames;
	Write["meta"]["name"] = Mems.Name;
	
	// Write frames
	auto& Frames = Write["frames"];
	for (auto& It : Mems.Frames)
	{
		if (It.second.size() == 0)
			continue;

		json Element = json();

		// Write current index
		Element["index"] = It.first;
		
		// Write actions
		auto& Actions = Element["actions"];
		for (auto& Ac : It.second)
		{
			json AcElement = json();
			
			AcElement["type"] = Ac->GetTypeName();
			Ac->Save(AcElement);

			//Ac.match(
			//	[&](SetHitbox O) {
			//		SaveOption(AcElement, O);
			//	},
			//	[&](SetAnimation O) {
			//		AcElement["type"] = "set_animation";
			//		AcElement["animation_file"] = O.AnimationFile;
			//		AcElement["offset_x"] = O.Offset.X.value;
			//		AcElement["offset_y"] = O.Offset.X.value;
			//		AcElement["starting_frame"] = O.StartingFrame;
			//	},
			//	[&](SetAnimationFrame O) {
			//		AcElement["type"] = "set_animation_frame";
			//		AcElement["frame"] = O.Frame;
			//	}
			//);

			Actions.push_back(AcElement);
		}

		// Add element to frames array
		Frames.push_back(Element);
	}

	// Save into file
	std::ofstream File;
	File.open(Filename);
	File << Write;
	File.close();
}

void game::AttackResource::Load(std::string Filename)
{
	// Load json
	std::ifstream File;
	File.open(Filename);

	if (!(bool)File)
	{
		std::cout << "Failed to load attack \"" << Filename << "\"" << std::endl;
	}

	// Into JSon
	std::string S((std::istreambuf_iterator<char>(File)),
		std::istreambuf_iterator<char>());
	const json Read = json::parse(S.begin(), S.end());

	// Clear members to be sure
	Mems.Name = "";
	Mems.Frames.clear();

	// Load metadata
	auto& Meta = Read["meta"];
	Mems.TotalFrames = Meta["total_frames"];
	std::string Name = Meta["name"];
	Mems.Name = Name;

	// Load frames
	auto& Frames = Read["frames"];
	for (auto& It : Frames)
	{
		int Index = It["index"];

		std::vector<AOption> Ops;
		auto& Actions = It["actions"];
		for (auto& Ac : Actions)
		{
			std::string Type = Ac["type"];
			
			auto* TypePtr = GetType(Type);
			if (TypePtr)
			{
				AOption Push(TypePtr->Create());
				Push->Load(Ac);
				Ops.push_back(std::move(Push));
			}
		}

		Mems.Frames[Index] = std::move(Ops);
	}
}

AttackResource::Members & game::AttackResource::Edit()
{
	return Mems;
}


