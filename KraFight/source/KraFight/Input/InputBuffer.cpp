#include "KraFight/Input/InputBuffer.h"

using namespace kra;

kra::InputBuffer::InputBuffer()
{
}

InputFrame & kra::InputBuffer::Edit()
{
	return Inputs[0];
}

void kra::InputBuffer::PushDown()
{
	for (size_t I = Inputs.size() - 1; I > 0; --I)
	{
		Inputs[I] = Inputs[I - 1];
	}
	Inputs[0] = InputFrame();
}

bool kra::InputBuffer::Pressed(Button InputFrame::* Button, int Depth, bool CheckForConsume) const
{
	for (size_t I = 0; I < Depth; ++I)
	{
		if ((Inputs[I].*Button).Consumed || (Inputs[I + 1].*Button).Consumed)
		{
			return false;
		}
		if ((Inputs[I].*Button).Held && !(Inputs[I + 1].*Button).Held)
		{
			return true;
		}
	}
	return false;
}

bool kra::InputBuffer::Release(Button InputFrame::* Button, int Depth, bool CheckForConsume) const
{
	for (size_t I = 0; I < Depth; ++I)
	{
		if ((Inputs[I].*Button).Consumed || (Inputs[I + 1].*Button).Consumed)
		{
			return false;
		}
		if (!(Inputs[I].*Button).Held && (Inputs[I + 1].*Button).Held)
		{
			return true;
		}
	}
	return false;
}

bool kra::InputBuffer::Held(Button InputFrame::* Button, int Depth, bool CheckForConsume) const
{
	for (size_t I = 0; I < Depth; ++I)
	{
		if ((Inputs[I].*Button).Consumed)
		{
			return false;
		}
		if ((Inputs[I].*Button).Held)
		{
			return true;
		}
	}
	return false;
}

int kra::InputBuffer::StickX(int Depth) const
{
	return Inputs[Depth].StickXNotNull ? ((int)Inputs[Depth].StickX + 1) * 2 - 3 : 0;
}

int kra::InputBuffer::StickY(int Depth) const
{
	return Inputs[Depth].StickYNotNull ? ((int)Inputs[Depth].StickY + 1) * 2 - 3 : 0;
}

void kra::InputBuffer::Consume(Button InputFrame::* Button)
{
	(Inputs[0].*Button).Consumed = true;
}

void kra::InputBuffer::ConsumeStick()
{
	Inputs[0].StickConsumed = true;
}
