#pragma once
#include "KraFight/Math/Vector2.h"

namespace kra {
	// Base Box struct used for both hit- and hurtboxes
	struct Box {
		Vector2 Position;
		Vector2 Size;
	};

	// Function to check of two boxes are overlapping
	bool Overlap(const Box& A, const Vector2& PosA, const Box& B, const Vector2& PosB);
}