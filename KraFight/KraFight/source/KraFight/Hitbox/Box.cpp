#include "Box.h"

bool kra::Overlap(const Box & A, const Vector2 & PosA, const Box & B, const Vector2 & PosB)
{
	kfloat Half = kfloat::makeFromFraction(1, 2);
	return (PosA.X + A.Position.X + A.Size.X * Half > PosB.X + B.Position.X - B.Size.X * Half &&
			PosA.X + A.Position.X - A.Size.X * Half < PosB.X + B.Position.X + B.Size.X * Half &&
			PosA.Y + A.Position.Y + A.Size.Y * Half > PosB.Y + B.Position.Y - B.Size.Y * Half &&
			PosA.Y + A.Position.Y - A.Size.Y * Half < PosB.Y + B.Position.Y + B.Size.Y * Half);
}
