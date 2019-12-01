#pragma once

namespace kra {
	enum class EPlayerStates {
		Idle,
		Crouch,
		Walk,
		Dash,
		JumpSquat,
		Jump,
		BlockHigh,
		BlockLow,
		BlockAir,
		GroundAttack,
		GroundSpecial,
		GroundSuper,
		AirAttack,
		AirSpecial,
		AirSuper,

		Hitstun,
		RecoveryGround,
		RecoveryAir,

		MAX,
	};
}