#pragma once
#include "KraFight/TypeDef.h"

namespace kra {
	struct Vector2 {
		kfloat X = 0.f;
		kfloat Y = 0.f;
	};

	Vector2 operator+(const Vector2& Vec1, const Vector2& Vec2);
	Vector2 operator-(const Vector2& Vec1, const Vector2& Vec2);
	Vector2 operator*(const Vector2& Vec1, const Vector2& Vec2);
	Vector2 operator/(const Vector2& Vec1, const Vector2& Vec2);
	Vector2 operator*(const Vector2& Vec1, kfloat Val);
	Vector2 operator/(const Vector2& Vec1, kfloat Val);
	Vector2& operator+=(Vector2& Vec1, const Vector2& Vec2);
	Vector2& operator-=(Vector2& Vec1, const Vector2& Vec2);
	Vector2& operator*=(Vector2& Vec1, const Vector2& Vec2);
	Vector2& operator/=(Vector2& Vec1, const Vector2& Vec2);
	Vector2& operator*=(Vector2& Vec1, kfloat Val);
	Vector2& operator/=(Vector2& Vec1, kfloat Val);
	Vector2 Normalize(const Vector2& Vec);
	kfloat Length(const Vector2& Vec);
}