#pragma once

namespace kra {
	enum class EPlayerStates {
		Idle,
		WalkBack,
		WalkForward,
		DashBack,
		DashForward,
		JumpSquat,
		Jump,
		BlockHigh,
		BlockLow,
		BlockAir,
		Attack,
		Special,

		MAX,
	};
}