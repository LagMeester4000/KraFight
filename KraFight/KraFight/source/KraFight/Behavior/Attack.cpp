#include "Attack.h"
#include "KraFight/Consts.h"
#include <assert.h>

using namespace kra;

AttackFrame & kra::Attack::operator[](size_t I)
{
	// Create new frame(s)
	while (I >= Frames.size())
	{
		Frames.push_back(AttackFrame());
	}
	return Frames[I];
}

void kra::Attack::ExecuteFrame(size_t I, const AttackContext & Context)
{
	if (I >= Frames.size())
		return;

	Frames[I].Execute(Context);
}

void kra::Attack::Run(kfloat OldTimer, kfloat NewTimer, const AttackContext & Context)
{
	assert(OldTimer < NewTimer);
	for (size_t I = (OldTimer / FrameTime).toInt(); I < (NewTimer / FrameTime).toInt(); ++I)
	{
		Frames[I].Execute(Context);
	}
}

size_t kra::Attack::Size()
{
	return Frames.size();
}

void kra::AttackFrame::Execute(const AttackContext & Context)
{
	for (auto& It : Actions)
	{
		It->Execute(Context);
	}
}
