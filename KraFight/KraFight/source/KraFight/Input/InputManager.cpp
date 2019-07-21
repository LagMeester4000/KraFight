#include "InputManager.h"

using namespace kra;

kra::InputManager::InputManager()
{
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

const InputBuffer & kra::InputManager::Get(Handle<InputBuffer> InputHandle)
{
	return PlayerInput[InputHandle.GetHandle()];
}
