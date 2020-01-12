#include "KraGame/Graphics/AnimationResource.h"
#include "KraGame/Detail/StringParse.h"
#include "json/json.hpp"
#include <fstream>
#include <assert.h>
#include <iostream>

using namespace game;
using json = nlohmann::json;

bool game::AnimationResource::Load(std::string Filename)
{
	// Load file
	std::ifstream I(Filename.c_str());

	// Check if file exists
	if (!(bool)I)
	{
		Loaded = false;
		std::cout << "Failed to load " << Filename << std::endl;
		return false;
	}

	// Into json object
	std::string S;
	I >> S;
	const json File = json::parse(S.begin(), S.end());

	// Clear old memory (to be sure)
	Frames.clear();

	// Find the subfolders in the json
	auto& JFrames = File["frames"];
	auto& Meta = File["meta"];

	// Load the image (and parse other metadata)
	{
		// Load width and height
		Width = Meta["size"]["w"];
		Height = Meta["size"]["h"];

		// Find image path
		std::string ImgFilename = Meta["image"];
		size_t LastSlash = str::FindLastInstanceOf('/', Filename);
		std::string FullImagePath;
		if (LastSlash == Filename.size())
		{
			// Slash not found
			FullImagePath = std::move(ImgFilename);
		}
		else 
		{
			// Slash found
			FullImagePath = std::string(Filename.begin(), Filename.begin() + LastSlash + 1);
			FullImagePath += ImgFilename;
		}

		// Load texture
		if (!Texture.loadFromFile(FullImagePath))
		{
			Loaded = false;
			std::cout << "Failed to load texure " << FullImagePath << std::endl;
			return false;
		}
	}

	// Parse the frames
	{
		// I have to assume that the frames are written/access in 
		//   right order, cause aseprite does not export it as an array
		for (auto& It : JFrames)
		{
			sf::IntRect Push;
			auto& FrameRect = It["frame"];
			Push.left = FrameRect["x"];
			Push.top = FrameRect["y"];
			Push.width = FrameRect["w"];
			Push.height = FrameRect["h"];
			Frames.push_back(Push);
		}
	}

	// Mark as loaded
	Loaded = true;
	return true;
}

const sf::IntRect & game::AnimationResource::GetFrame(size_t Frame) const
{
	assert(Frame >= Frames.size());
	return Frames[Frame];
}

const sf::Texture & game::AnimationResource::GetTexture() const
{
	return Texture;
}

int game::AnimationResource::GetTotalWidth() const
{
	return Width;
}

int game::AnimationResource::GetTotalHeight() const
{
	return Height;
}

bool game::AnimationResource::IsLoaded() const
{
	return Loaded;
}
