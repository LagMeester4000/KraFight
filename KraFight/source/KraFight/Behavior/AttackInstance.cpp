#include "KraFight/Behavior/AttackInstance.h"
#include "KraFight/Behavior/Attack.h"
#include "KraFight/Resource/ResourceManager.h"
#include "KraFight/Network/NetLoadBuffer.h"
#include "KraFight/Network/NetSaveBuffer.h"

using namespace kra;

kra::AttackInstance::AttackInstance()
	: Active(false), Timer(0_k)
{
}

void kra::AttackInstance::Update(kfloat DeltaTime, const AttackContext & Con)
{
	if (!Active)
		return;

	kfloat NewTimer = Timer + DeltaTime;
	auto NewTimerFrame = (NewTimer / FrameTime).toInt();
	auto OldTimerFrame = (Timer / FrameTime).toInt();
	Timer = NewTimer;

	// Only continue if the current frame is a new frame
	if (OldTimerFrame >= NewTimerFrame)
		return;

	// Get the current attack
	auto* At = Con.Context.Resources->Attacks.GetResource(CurrentAttack);

	// Safety switch
	if (!At)
	{
		Active = false;
		return;
	}

	// Check if the attack should end
	if (NewTimerFrame > At->Size()) //== is fine for last frame, > is over the limit
	{
		// Attack is over
		Active = false;
		return;
	}

	// Probably only going to run for one iteration
	for (auto I = OldTimerFrame; I < NewTimerFrame; ++I)
	{
		At->ExecuteFrame((size_t)I, Con);
	}
}

void kra::AttackInstance::Activate(Handle<kra::Attack> NewAttack)
{
	Active = true;
	CurrentAttack = NewAttack;
	Timer = 0_k;
}

void kra::AttackInstance::Deactivate()
{
	Active = false;
}

bool kra::AttackInstance::IsActive() const
{
	return Active;
}

void kra::AttackInstance::NetSave(NetSaveBuffer & Buff)
{
	Buff << Active
		<< Timer
		<< CurrentAttack;
}

void kra::AttackInstance::NetLoad(NetLoadBuffer & Buff)
{
	Buff >> Active
		>> Timer
		>> CurrentAttack;
}
