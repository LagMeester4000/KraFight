#pragma once
#include <stdint.h>

namespace kra {
	class FixedPoint {
	public:
		FixedPoint();
		FixedPoint(int32_t InValue);
		FixedPoint(float InValue);
		FixedPoint(const FixedPoint &OtherFixedPoint);

		// Arithmetic operators
		FixedPoint& operator=(int32_t IntValue);
		FixedPoint& operator=(FixedPoint OtherFixedPoint);
		FixedPoint operator+(FixedPoint OtherFixedPoint) const;
		FixedPoint operator-(FixedPoint OtherFixedPoint) const;
		FixedPoint operator*(FixedPoint OtherFixedPoint) const;
		FixedPoint operator/(FixedPoint OtherFixedPoint) const;
		FixedPoint& operator+=(FixedPoint OtherFixedPoint);
		FixedPoint& operator-=(FixedPoint OtherFixedPoint);
		FixedPoint& operator*=(FixedPoint OtherFixedPoint);
		FixedPoint& operator/=(FixedPoint OtherFixedPoint);
		FixedPoint operator-();

		// Compare operators
		bool operator>(FixedPoint Other);
		bool operator>=(FixedPoint Other);
		bool operator<(FixedPoint Other);
		bool operator<=(FixedPoint Other);
		bool operator==(FixedPoint Other);
		bool operator!=(FixedPoint Other);
		bool operator!();
		operator bool();

		// Conversion functions
		static FixedPoint CreateFromInt(int32_t Value);
		static FixedPoint CreateFromFloat(float Value);
		static FixedPoint CreateFromFraction(int32_t Whole, int32_t Div);
		float toFloat();

		// Calculate the root of a fixed point value
		static FixedPoint Root(FixedPoint Value);
		FixedPoint Root();

		// Floor the value
		static FixedPoint Floor(FixedPoint Value);
		FixedPoint Floor();

		// Ceil the value
		static FixedPoint Ceil(FixedPoint Value);
		FixedPoint Ceil();

		int32_t Value;
		static const int32_t FractionBits = 12;
		static const int32_t FirstIntegerBitSet = 1 << FractionBits;

	public:
		static bool UnitTest();
	};

	// Duplicate of root function
	FixedPoint sqrt(FixedPoint Point);

	// Custom postfix
	FixedPoint operator "" k(uint64_t Num);
}