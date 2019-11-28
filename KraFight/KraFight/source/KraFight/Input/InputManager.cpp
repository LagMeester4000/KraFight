#include "InputManager.h"

using namespace kra;

kra::InputManager::InputManager()
{
}

void kra::InputManager::Update()
{
	if (bInputEnabled)
	{
		PushDown();
	}
}

void kra::InputManager::PushDown()
{
	PlayerInput[0].PushDown();
	PlayerInput[1].PushDown();
}

InputFrame & kra::InputManager::Edit(Handle<InputBuffer> InputHandle)
{
	return PlayerInput[InputHandle.GetHandle()].Edit();
}

void kra::InputManager::Insert(Handle<InputBuffer> InputHandle, const InputFrame & Frame)
{
	PlayerInput[InputHandle.GetHandle()].PushDown();
	PlayerInput[InputHandle.GetHandle()].Edit() = Frame;
}

const InputBuffer & kra::InputManager::Get(Handle<InputBuffer> InputHandle) const
{
	return PlayerInput[InputHandle.GetHandle()];
}

void kra::InputManager::SetInputEnabled(bool bVal)
{
	bInputEnabled = bVal;
}

bool kra::InputManager::IsInputEnabled() const
{
	return bInputEnabled;
}

Handle<InputBuffer> kra::InputManager::MakeHandle(int PlayerIndex) const
{
	Handle<InputBuffer> Ret;
	Ret.SetHandle(PlayerIndex);
	return Ret;
}
