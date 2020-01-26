#include "KraNet/InputBuffer.h"
#include "KraNet/Consts.h"
#include "KraNet/Packet.h"

kra::net::InputBuffer::InputBuffer()
{
	// Each input buffer starts with one valid empty input to make frame count 0
	Element Push;
	Push.Valid = true;
	//Push.Input should have values as 0 by default
	Inputs.PushBack(Push);
}

void kra::net::InputBuffer::PushInput(const KraNetInput & Input)
{
	//Input
}

void kra::net::InputBuffer::InsertInput(const KraNetInput & Input, i32 Frame)
{
}

KraNetInput kra::net::InputBuffer::GetInput(i32 Frame) const
{
	return GetLastValidInput(Frame);
}

KraNetInput kra::net::InputBuffer::GetLastValidInput(i32 Frame) const
{
	if (Frame < 0)
	{
		return KraNetInput{ 0, 0, 0, 0 };
	}

	i32 LowestExisting = Inputs.Bottom();
	i32 Top = Inputs.Top();

	if (Top == 0)
	{
		return KraNetInput{ 0, 0, 0, 0 };
	}

	// Frame does not exist anymore
	if (Frame < LowestExisting)
	{
		return KraNetInput{ 0, 0, 0, 0 };
	}

	// Frame does not exist yet, take last valid frame
	if (Frame >= Top)
	{
		Frame = Top - 1;
	}

	// Loop until last valid input is found
	while (!IsInputValid(Frame) && Frame > LowestExisting)
	{
		--Frame;
	}
	
	return Inputs[(size_t)Frame].Input;
}

bool kra::net::InputBuffer::IsInputValid(i32 Frame) const
{
	if (Frame < 0 || Frame >= Inputs.Top() || Frame < Inputs.Bottom())
	{
		return false;
	}
	return Inputs[Frame].Valid;
}

bool kra::net::InputBuffer::IsInputRangeValid(i32 StartFrame, i32 EndFrame) const
{
	return (StartFrame >= Inputs.Bottom() && EndFrame < Inputs.Top() && 
		StartFrame >= EndFrame);
}

i32 kra::net::InputBuffer::GetFrame() const
{
	return Inputs.Top() - 1;
}

bool kra::net::InputBuffer::CheckForGaps()
{
	// Only check in a restricted region
	i32 Region = MaxRollbackFrames + MaxFlatInputDelay + 1;

	// I know a gap exists if there are newer confirmed frames
	//   and older frames that are not confirmed
	// Therefore I only have to reverse iterate until I find a non confirmed
	//   frame before a confirmed frame
	for (i32 I = 0; I < Region; ++I)
	{
		i32 Frame = Inputs.Top() - 1 - I;
		auto& It = Inputs[Frame];
		if (!It.Valid)
		{
			return true;
		}
	}

	return false;
}

void kra::net::InputBuffer::SaveToPacket(Packet & Pack)
{
	i32 InputAmount = PacketInputBufferSize;
	if (Inputs.Size() < InputAmount)
	{
		InputAmount = Inputs.Size();
	}
	
	Pack.Type = PacketType::GameInput;
	Pack.Pack.GameInput.InputSize = InputAmount;
	Pack.Pack.GameInput.Frame = (i32)(Inputs.Top() - 1);

	// Insert inputs into packet
	for (i32 I = 0; I < InputAmount; ++I)
	{
		auto& It = Inputs.GetFromTop(I);
		Pack.Pack.GameInput.Inputs[I] = It.Input;
		Pack.Pack.GameInput.InputsValid[I] = It.Valid;
	}
}

void kra::net::InputBuffer::SaveOldInputToPacket(Packet & Pack, i32 Frame)
{
	// Check if request is invalid
	if (Frame >= Inputs.Top())
	{
		// Do normal save
		SaveToPacket(Pack);
		return;
	}

	i32 InputAmount = PacketInputBufferSize;
	if (Inputs.Top() <= Frame)
	{
		// Error
		// Idk what to do here
	}
	else if (Frame < InputAmount)
	{
		InputAmount = Frame + 1;
	}

	Pack.Type = PacketType::GameInput;
	Pack.Pack.GameInput.InputSize = InputAmount;
	Pack.Pack.GameInput.Frame = Frame;

	i32 Bottom = (i32)Inputs.Bottom();
	for (i32 I = 0; I < InputAmount; ++I)
	{
		i32 FrameI = Frame - I;
		if (FrameI >= Bottom)
		{
			auto& It = Inputs[(size_t)FrameI];
			Pack.Pack.GameInput.Inputs[FrameI] = It.Input;
			Pack.Pack.GameInput.InputsValid[FrameI] = It.Valid;
		}
		else
		{
			Pack.Pack.GameInput.InputsValid[FrameI] = false;
		}
	}
}

bool kra::net::InputBuffer::LoadFromPacket(const Packet & Pack)
{
	if (Pack.Type == PacketType::GameInput)
	{
		i32 BaseFrame = Pack.Pack.GameInput.Frame;
		i32 ISize = Pack.Pack.GameInput.InputSize;
		for (i32 I = 0; I < ISize; ++I)
		{
			if (Pack.Pack.GameInput.InputsValid[I])
			{
				// Insert valid input
				auto& It = Pack.Pack.GameInput.Inputs[I];
				Element Push{ true, It };
				Element* Inv = Inputs.Insert(Push, (size_t)(BaseFrame - I));
				if (Inv)
				{
					Inv->Valid = false;
				}
			}
		}

		return true;
	}
	return false;
}
