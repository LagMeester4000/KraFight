#include "KraGame/Graphics/ResourceManager.h"
#include "KraGame/Graphics/AnimationResource.h"
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <utility>
#include <assert.h>

using namespace game;

game::ResourceManager::ResourceManager()
{
}

game::ResourceManager::~ResourceManager()
{
}

kra::Handle<sf::Texture> game::ResourceManager::LoadTexture(std::string Filename)
{
	auto Find = TextureHandles.find(Filename);
	if (Find != TextureHandles.end())
	{
		return Find->second;
	}

	auto Push = std::make_unique<sf::Texture>();
	if (!Push->loadFromFile(Filename))
	{
		std::cout << "Could not load texture " << Filename << std::endl;
		return kra::Handle<sf::Texture>();
	}

	size_t Ret = Textures.size();
	Textures.push_back(std::move(Push));
	auto RetHandle = kra::Handle<sf::Texture>((kra::HandleT)Ret);
	TextureHandles[Filename] = RetHandle;
	return RetHandle;
}

sf::Texture & game::ResourceManager::GetTexture(kra::Handle<sf::Texture> Hand)
{
	assert(Hand.IsValid());
	return *Textures[Hand.GetHandle()];
}

kra::Handle<AnimationResource> game::ResourceManager::LoadAnimation(std::string Filename)
{
	auto Find = AnimationHandles.find(Filename);
	if (Find != AnimationHandles.end())
	{
		return Find->second;
	}

	auto Push = std::make_unique<AnimationResource>();
	if (!Push->Load(Filename))
	{
		return kra::Handle<AnimationResource>();
	}

	size_t Ret = Animations.size();
	Animations.push_back(std::move(Push));
	auto RetHandle = kra::Handle<AnimationResource>((kra::HandleT)Ret);
	AnimationHandles[Filename] = RetHandle;
	return RetHandle;
}

AnimationResource & game::ResourceManager::GetAnimation(kra::Handle<AnimationResource> Hand)
{
	assert(Hand.IsValid());
	return *Animations[Hand.GetHandle()];
}
