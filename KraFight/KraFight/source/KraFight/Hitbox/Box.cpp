#include "Box.h"

bool kra::Overlap(const Box & A, const Vector2 & PosA, const Box & B, const Vector2 & PosB)
{
	kfloat Half = kfloat::CreateFromFraction(1, 2);
	return (PosA.X + A.Position.X + A.Size.X * Half < PosB.X + B.Position.X - A.Size.X * Half &&
			PosA.X + A.Position.X - A.Size.X * Half > PosB.X + B.Position.X + A.Size.X * Half &&
			PosA.Y + A.Position.Y + A.Size.Y * Half < PosB.Y + B.Position.Y - A.Size.Y * Half &&
			PosA.Y + A.Position.Y - A.Size.Y * Half > PosB.Y + B.Position.Y + A.Size.Y * Half);
}
