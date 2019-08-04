#include "FixedPoint.h"
#include <math.h>

using namespace kra;

FixedPoint::FixedPoint()
	: Value{ 0 }
{
}

FixedPoint::FixedPoint(int32_t InValue)
	: Value{ InValue }
{
}

kra::FixedPoint::FixedPoint(float InValue)
{
	*this = CreateFromFloat(InValue);
}

FixedPoint::FixedPoint(const FixedPoint &OtherFixedPoint)
{
	Value = OtherFixedPoint.Value;
}

FixedPoint& FixedPoint::operator=(FixedPoint OtherFixedPoint)
{
	Value = OtherFixedPoint.Value;
	return *this;
}

FixedPoint& FixedPoint::operator=(int32_t IntValue)
{
	Value = IntValue << FractionBits;
	return *this;
}

FixedPoint FixedPoint::operator+(FixedPoint otherFixedPoint) const
{
	int32_t Result = Value + otherFixedPoint.Value;
	return FixedPoint(Result);
}

FixedPoint FixedPoint::operator-(FixedPoint OtherFixedPoint) const
{
	int32_t Result = Value - OtherFixedPoint.Value;
	return FixedPoint(Result);
}

FixedPoint FixedPoint::operator*(FixedPoint OtherFixedPoint) const
{
	int32_t Result = Value * OtherFixedPoint.Value;
	
	// rounding of last bit, can be removed for performance
	//result = result + ((result & 1 << (FractionBits - 1)) << 1);
	
	Result = Result >> FractionBits;
	return FixedPoint(Result);
}

FixedPoint FixedPoint::operator/(FixedPoint OtherFixedPoint) const
{
	int32_t Result = (Value << FractionBits) / OtherFixedPoint.Value;
	return FixedPoint(Result);
}

FixedPoint & kra::FixedPoint::operator+=(FixedPoint OtherFixedPoint)
{
	*this = *this + OtherFixedPoint;
	return *this;
}

FixedPoint & kra::FixedPoint::operator-=(FixedPoint OtherFixedPoint)
{
	*this = *this - OtherFixedPoint;
	return *this;
}

FixedPoint & kra::FixedPoint::operator*=(FixedPoint OtherFixedPoint)
{
	*this = *this * OtherFixedPoint;
	return *this;
}

FixedPoint & kra::FixedPoint::operator/=(FixedPoint OtherFixedPoint)
{
	*this = *this / OtherFixedPoint;
	return *this;
}

FixedPoint kra::FixedPoint::operator-()
{
	FixedPoint Ret;
	Ret.Value = -Value;
	return Ret;
}

bool kra::FixedPoint::operator>(FixedPoint Other)
{
	return Value > Other.Value;
}

bool kra::FixedPoint::operator>=(FixedPoint Other)
{
	return Value >= Other.Value;
}

bool kra::FixedPoint::operator<(FixedPoint Other)
{
	return Value < Other.Value;
}

bool kra::FixedPoint::operator<=(FixedPoint Other)
{
	return Value <= Other.Value;
}

bool kra::FixedPoint::operator==(FixedPoint Other)
{
	return Value == Other.Value;
}

bool kra::FixedPoint::operator!=(FixedPoint Other)
{
	return Value != Other.Value;
}

bool kra::FixedPoint::operator!()
{
	return Value != 0;
}

kra::FixedPoint::operator bool()
{
	return (bool)Value;
}

FixedPoint FixedPoint::CreateFromInt(int32_t Value)
{
	int32_t NewValue = Value << FractionBits;
	return FixedPoint(NewValue);
}

FixedPoint FixedPoint::CreateFromFloat(float Value)
{
	int32_t NewValue = Value * FirstIntegerBitSet;
	return FixedPoint(NewValue);
}

FixedPoint kra::FixedPoint::CreateFromFraction(int32_t Whole, int32_t Div)
{
	int32_t IVal = 1 << FractionBits;
	IVal = IVal * Whole / Div;

	return FixedPoint(IVal);
}

float kra::FixedPoint::toFloat()
{
	return (float)Value / powf(2.f, (float)FractionBits);
}

FixedPoint kra::FixedPoint::Root(FixedPoint X)
{
	if (X.Value < 0)
	{
		return FixedPoint(-1);
	}

	uint32_t t, q, b, r;
	r = X.Value;
	b = 0x40000000;
	q = 0;
	while (b > 0x10)
	{
		t = q + b;
		if (r >= t)
		{
			r -= t;
			q = t + b; // equivalent to q += 2*b
		}
		r <<= 1;
		b >>= 1;
	}
	q >>= 10;
	return (int32_t)q;
}

FixedPoint kra::FixedPoint::Root()
{
	return Root(*this);
}

FixedPoint kra::FixedPoint::Floor(FixedPoint Value)
{
	auto Change = Value.Value >> FractionBits;
	Change <<= FractionBits;

	return FixedPoint(Change);
}

FixedPoint kra::FixedPoint::Floor()
{
	return Floor(*this);
}

FixedPoint kra::FixedPoint::Ceil(FixedPoint Value)
{
	auto Fl = Floor(Value);
	auto One = CreateFromInt(1);

	return Fl + One;
}

FixedPoint kra::FixedPoint::Ceil()
{
	return Ceil(*this);
}

bool kra::FixedPoint::UnitTest()
{
	// Test for bit shift operators
	{
		int32_t I = -2;
		I >>= 1;
		if (I >= 0) // Very big error
			return false;
	}

	// Test for mismatching positive/negative vals
	{
		FixedPoint MinusTwo = FixedPoint::CreateFromFloat(-2.f);
		FixedPoint Four = FixedPoint::CreateFromFloat(4.f);

		auto Res0 = Four / MinusTwo;

	}

	// Test for simple exact calculations
	{
		FixedPoint One = FixedPoint(1 << FractionBits);
		if (One.toFloat() != 1.f)
			return false;
	}

	return true;
}

FixedPoint kra::sqrt(FixedPoint Point)
{
	return FixedPoint::Root(Point);
}

FixedPoint kra::operator""_k(uint64_t Num)
{
	return FixedPoint::CreateFromInt((int32_t)Num);
}
