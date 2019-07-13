#include "InputFrame.h"
#include <bitset>
#include <Bits.h>

using namespace kra;

int32_t kra::ToInt(const InputFrame & In)
{
	std::bitset<32> Conv;

	Conv[0] = In.StickX;
	Conv[1] = In.StickXNotNull;
	Conv[2] = In.StickY;
	Conv[3] = In.StickYNotNull;

	Conv[4] = In.Attack1.Held;
	Conv[5] = In.Attack2.Held;
	Conv[6] = In.Attack3.Held;
	Conv[7] = In.Special.Held;
	Conv[8] = In.Grab.Held;

	int32_t Ret = *(int32_t*)&Conv;
	return Ret;
}

InputFrame kra::FromInt(int32_t In)
{
	std::bitset<32> Conv = In;
	InputFrame Ret;

	Ret.StickX = Conv[0];
	Ret.StickXNotNull = Conv[1];
	Ret.StickY = Conv[2];
	Ret.StickYNotNull = Conv[3];
	Ret.Attack1.Held = Conv[4];
	Ret.Attack2.Held = Conv[5];
	Ret.Attack3.Held = Conv[6];
	Ret.Special.Held = Conv[7];
	Ret.Grab.Held = Conv[8];

	return Ret;
}
