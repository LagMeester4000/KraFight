#include "FixedPoint.h"
#include <math.h>

using namespace kra;

FixedPoint::FixedPoint()
	: value{ 0 }
{
}

FixedPoint::FixedPoint(int32_t inValue)
	: value{ inValue }
{
}

FixedPoint::FixedPoint(const FixedPoint &otherFixedPoint)
{
	value = otherFixedPoint.value;
}

FixedPoint& FixedPoint::operator=(FixedPoint otherFixedPoint)
{
	value = otherFixedPoint.value;
	return *this;
}

FixedPoint& FixedPoint::operator=(int32_t intValue)
{
	value = intValue << FractionBits;
	return *this;
}

FixedPoint FixedPoint::operator+(FixedPoint otherFixedPoint) const
{
	int32_t result = value + otherFixedPoint.value;
	return FixedPoint(result);
}

FixedPoint FixedPoint::operator-(FixedPoint otherFixedPoint) const
{
	int32_t result = value - otherFixedPoint.value;
	return FixedPoint(result);
}

FixedPoint FixedPoint::operator*(FixedPoint otherFixedPoint) const
{
	int32_t result = value * otherFixedPoint.value;
	
	// rounding of last bit, can be removed for performance
	//result = result + ((result & 1 << (FractionBits - 1)) << 1);
	
	result = result >> FractionBits;
	return FixedPoint(result);
}

FixedPoint FixedPoint::operator/(FixedPoint otherFixedPoint) const
{
	int32_t result = (value << FractionBits) / otherFixedPoint.value;
	return FixedPoint(result);
}

FixedPoint FixedPoint::createFromInt(int32_t value)
{
	int32_t newValue = value << FractionBits;
	return FixedPoint(newValue);
}

FixedPoint FixedPoint::createFromFloat(float value)
{
	int32_t newValue = value * FirstIntegerBitSet;
	return FixedPoint(newValue);
}

float kra::FixedPoint::toFloat()
{
	return (float)value / powf(2.f, (float)FractionBits);
}

FixedPoint kra::FixedPoint::Root(FixedPoint x)
{
	if (x.value < 0)
	{
		return FixedPoint(-1);
	}

	uint32_t t, q, b, r;
	r = x.value;
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
	return q;
}
