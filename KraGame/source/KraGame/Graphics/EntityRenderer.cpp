#include "KraGame/Graphics/EntityRenderer.h"
#include "KraGame/Graphics/ResourceManager.h"
#include "KraGame/Graphics/AnimationResource.h"
#include <KraFight/Context.h>
#include <KraFight/Entity/EntityManager.h>
#include <KraFight/Entity/Entity.h>
#include <KraFight/Physics/PhysicsManager.h>
#include <KraFight/Network/NetSaveBuffer.h>
#include <KraFight/Network/NetLoadBuffer.h>
#include <SFML/Graphics/RenderWindow.hpp>

game::EntityRenderer::EntityRenderer()
	: Offset(kra::Vector2{ 0, 0 })
{
}

void game::EntityRenderer::Render(const kra::Context & Con, kra::Handle<kra::Entity> Ent, 
	sf::RenderWindow & Window, ResourceManager & Resources)
{
	if (!Enabled || !Anim.IsValid())
	{
		return;
	}

	// Get KraFight data
	auto& Enti = Con.Entities->Get(Ent);
	auto& Phys = Con.PhysicsObjects->Get(Enti->GetPhysicsBody());
	
	// Find position
	kra::Vector2 DrawPos = Phys.GetPosition() + Offset;
	sf::Vector2f DrawPosF = { kra::toFloat<float>(DrawPos.X), kra::toFloat<float>(DrawPos.Y) };

	Sprite.setPosition(DrawPosF);
	Window.draw(Sprite);
}

void game::EntityRenderer::SetEnabled(bool B)
{
	Enabled = B;
}

void game::EntityRenderer::SetAnimation(kra::Handle<AnimationResource> NewAnim, ResourceManager & Resources)
{
	if (NewAnim != Anim)
	{
		Anim = NewAnim;
		auto& Animation = Resources.GetAnimation(Anim);
		Sprite.setTexture(Animation.GetTexture(), true);
	}
}

void game::EntityRenderer::SetAnimationFrame(int NewAnim, ResourceManager & Resources)
{
	if (NewAnim != AnimFrame)
	{
		AnimFrame = NewAnim;
		auto& Animation = Resources.GetAnimation(Anim);
		auto& Rect = Animation.GetFrame(AnimFrame);
		Sprite.setTextureRect(Rect);
		Sprite.setOrigin(Rect.width / 2.f, Rect.height / 2.f);
	}
}

void game::EntityRenderer::NetSave(kra::NetSaveBuffer & Buff)
{
	Buff << Enabled 
		<< Flipped
		<< Offset
		<< Anim
		<< AnimFrame;
}

void game::EntityRenderer::NetLoad(kra::NetLoadBuffer & Buff, ResourceManager & Resources)
{
	// Might want to reload these
	kra::Handle<AnimationResource> NewAnim;
	int NewAnimFrame;

	// Load actual data from buffer
	Buff >> Enabled
		>> Flipped
		>> Offset
		>> NewAnim
		>> NewAnimFrame;

	// Try to load animations
	if (NewAnim != Anim)
	{
		SetAnimation(NewAnim, Resources);
	}
	if (NewAnimFrame != AnimFrame)
	{
		SetAnimationFrame(NewAnimFrame, Resources);
	}
}
