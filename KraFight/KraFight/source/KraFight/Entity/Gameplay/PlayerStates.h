#pragma once

namespace kra {
	enum class EPlayerStates {
		Idle,
		Walk,
		Dash,
		JumpSquat,
		Jump,
		BlockHigh,
		BlockLow,
		BlockAir,
		Attack,
		Special,
		Super,

		Hitstun,

		MAX,
	};
}