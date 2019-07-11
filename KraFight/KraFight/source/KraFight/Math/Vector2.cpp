#include "Vector2.h"
#include <math.h>

using namespace kra;

Vector2 kra::operator+(const Vector2 & Vec1, const Vector2 & Vec2)
{
	return { Vec1.X + Vec2.X, Vec1.Y + Vec2.Y };
}

Vector2 kra::operator-(const Vector2 & Vec1, const Vector2 & Vec2)
{
	return { Vec1.X - Vec2.X, Vec1.Y - Vec2.Y };
}

Vector2 kra::operator*(const Vector2 & Vec1, const Vector2 & Vec2)
{
	return { Vec1.X * Vec2.X, Vec1.Y * Vec2.Y };
}

Vector2 kra::operator/(const Vector2 & Vec1, const Vector2 & Vec2)
{
	return { Vec1.X / Vec2.X, Vec1.Y / Vec2.Y };
}

Vector2 kra::operator*(const Vector2 & Vec1, kfloat Val)
{
	return { Vec1.X * Val, Vec1.Y * Val };
}

Vector2 kra::operator/(const Vector2 & Vec1, kfloat Val)
{
	return { Vec1.X / Val, Vec1.Y / Val };
}

Vector2& kra::operator+=(Vector2 & Vec1, const Vector2 & Vec2)
{
	Vec1 = { Vec1.X + Vec2.X, Vec1.Y + Vec2.Y };
	return Vec1;
}

Vector2& kra::operator-=(Vector2 & Vec1, const Vector2 & Vec2)
{
	Vec1 = { Vec1.X - Vec2.X, Vec1.Y - Vec2.Y };
	return Vec1;
}

Vector2& kra::operator*=(Vector2 & Vec1, const Vector2 & Vec2)
{
	Vec1 = { Vec1.X * Vec2.X, Vec1.Y * Vec2.Y };
	return Vec1;
}

Vector2& kra::operator/=(Vector2 & Vec1, const Vector2 & Vec2)
{
	Vec1 = { Vec1.X / Vec2.X, Vec1.Y / Vec2.Y };
	return Vec1;
}

Vector2& kra::operator*=(Vector2 & Vec1, kfloat Val)
{
	Vec1 = { Vec1.X * Val, Vec1.Y * Val };
	return Vec1;
}

Vector2& kra::operator/=(Vector2 & Vec1, kfloat Val)
{
	Vec1 = { Vec1.X / Val, Vec1.Y / Val };
	return Vec1;
}

Vector2 kra::Normalize(const Vector2 & Vec)
{
	auto Div = Length(Vec);
	return Vec / Div;
}

kfloat kra::Length(const Vector2 & Vec)
{
	kfloat Ret = Vec.X * Vec.X + Vec.Y * Vec.Y;
	Ret = sqrtf(Ret);
	return Ret;
}
