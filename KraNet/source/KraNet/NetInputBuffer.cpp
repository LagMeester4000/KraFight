#include "KraNet/NetInputBuffer.h"
#include <assert.h>

using namespace kra::net;

kra::net::NetInputBuffer::NetInputBuffer()
	: CurrentGameplayFrame(0), MaxNetworkFrame(0), MaxLocalFrame(0)
{
}

void kra::net::NetInputBuffer::Reset()
{
	CurrentGameplayFrame = 0;
	MaxNetworkFrame = 0;
	for (auto& It : Inputs)
	{
		It = NetInputBufferElement();
	}
}

bool kra::net::NetInputBuffer::CanAdvanceGameplayFrame() const
{
	return IsValid(CurrentGameplayFrame);
}

bool kra::net::NetInputBuffer::AdvanceGameplayFrame()
{
	if (!IsValid(CurrentGameplayFrame))
		return false;

	// Invalidate new element
	Inputs[(size_t)CurrentGameplayFrame % Inputs.size()] = NetInputBufferElement();

	CurrentGameplayFrame++;

	return true;
}

const InputPair& kra::net::NetInputBuffer::GetGameplayFrame() const
{
	return AccessConst((size_t)CurrentGameplayFrame).Input;
}

void kra::net::NetInputBuffer::InsertNetworkFrame(FrameT NewFrame, KraNetInput Input)
{
	assert(NewFrame >= LowestFrame());

	FrameT NewSize = NewFrame - LowestFrame() + 1;
	if (NewSize > Inputs.size())
	{
		ResizeAndFit((size_t)NewSize);
	}

	if (NewFrame > MaxNetworkFrame)
	{
		MaxNetworkFrame = NewFrame;
	}

	// Do actual insertion
	auto& In = Access((size_t)NewFrame);
	In.RemoteValid = true;
	In.Input.Remote = Input;
}

void kra::net::NetInputBuffer::InsertLocalFrame(FrameT Delay, KraNetInput Input)
{
	FrameT NewFrame = CurrentGameplayFrame + Delay;
	assert(NewFrame >= LowestFrame());

	FrameT NewSize = NewFrame - LowestFrame() + 1;
	if (NewSize > Inputs.size())
	{
		ResizeAndFit((size_t)NewFrame);
		MaxNetworkFrame = NewFrame;
	}

	if (NewFrame > MaxLocalFrame)
	{
		MaxLocalFrame = NewFrame;
	}

	// Do actual insertion
	auto& In = Access((size_t)NewFrame);
	In.LocalValid = true;
	In.Input.Local = Input;

	// If any frames are skipped, fill them with this new input
	/*for (size_t I = NewFrame - 1; I >= CurrentGameplayFrame; --I)
	{
		auto& In = Inputs[(size_t)I];
		if (!In.LocalValid)
		{
			In.LocalValid = true;
			In.Input.Local = Input;
		}
		else
		{
			break;
		}
	}*/
}

void kra::net::NetInputBuffer::InitializeLocalFrames(FrameT Delay)
{
	for (FrameT I = 0; I < Delay; ++I)
	{
		Access(I).LocalValid = true;
	}
	MaxLocalFrame = Delay;
}

void kra::net::NetInputBuffer::SetSize(size_t NewSize)
{
	Reset();
	Inputs.resize(NewSize);
}

const InputPair & kra::net::NetInputBuffer::GetInput(FrameT Frame) const
{
	assert(BufferValid());
	assert(IsInRange(Frame));

	return Inputs[(size_t)Frame % Inputs.size()].Input;
}

bool kra::net::NetInputBuffer::IsValid(FrameT Frame) const
{
	auto& In = Inputs[(size_t)Frame % Inputs.size()];
	return BufferValid() && 
		IsInRange(Frame) &&
		In.LocalValid && In.RemoteValid;
}

bool kra::net::NetInputBuffer::IsInRange(FrameT Frame) const
{
	return Frame <= Max(MaxNetworkFrame, MaxLocalFrame) &&
		Frame >= LowestFrame();
}

NetInputBuffer::FrameT kra::net::NetInputBuffer::LowestFrame() const
{
	return CurrentGameplayFrame;
}

NetInputBuffer::FrameT kra::net::NetInputBuffer::GetGameplayFrameIndex() const
{
	return CurrentGameplayFrame;
}

void kra::net::NetInputBuffer::MakeSendableInputBuffer(std::vector<KraNetInput>& Buff, FrameT & StartingFrame) const
{
	// Make sure the buffer is cleared
	Buff.clear();

	StartingFrame = CurrentGameplayFrame;

	FrameT I = CurrentGameplayFrame;
	const auto* It = &AccessConst(I);
	while (It->LocalValid)
	{
		Buff.push_back(It->Input.Local);

		I++;
		if (!IsInRange(I))
			break;
		It = &AccessConst(I);
	}
}

void kra::net::NetInputBuffer::ReadReceivedInputBuffer(const std::vector<KraNetInput>& Buff, FrameT StartingFrame)
{
	for (size_t I = 0; I < Buff.size(); ++I)
	{
		FrameT Frame = StartingFrame + (FrameT)I;
		if (!IsInRange(Frame))
			continue;

		InsertNetworkFrame(Frame, Buff[I]);
	}
}

bool kra::net::NetInputBuffer::BufferValid() const
{
	return Inputs.size() != 0;
}

void kra::net::NetInputBuffer::ResizeAndFit(size_t NewSize)
{
	if (NewSize <= Inputs.size())
		return;

	NetInputBufferElement Def = NetInputBufferElement();
	std::vector<NetInputBufferElement> NewVec(NewSize, Def);

	for (FrameT I = LowestFrame(); I <= MaxNetworkFrame; ++I)
	{
		Access(NewVec, (size_t)I) = Access(Inputs, (size_t)I);
	}

	// Move new container into Inputs
	Inputs = std::move(NewVec);
}

NetInputBufferElement & kra::net::NetInputBuffer::Access(size_t Frame)
{
	return Inputs[Frame % Inputs.size()];
}

const NetInputBufferElement & kra::net::NetInputBuffer::AccessConst(size_t Frame) const
{
	return Inputs[Frame % Inputs.size()];
}

NetInputBufferElement & kra::net::NetInputBuffer::Access(std::vector<NetInputBufferElement>& Inputs, size_t Frame)
{
	return Inputs[Frame % Inputs.size()];
}

NetInputBuffer::FrameT kra::net::NetInputBuffer::Max(FrameT f1, FrameT f2)
{
	return f1 > f2 ? f1 : f2;
}
