#include "KraFight/Math/FixedPoint.h"

using namespace kra;

FixedPoint sqrtF2F(FixedPoint x)
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

int main()
{
	FixedPoint half = FixedPoint::createFromFloat(0.5f);

	FixedPoint fp = FixedPoint::createFromFloat(2.5f);
	FixedPoint fp2 = FixedPoint::createFromFloat(-6.6f);
	auto res = fp2 / fp;
	auto f = res.toFloat();

	FixedPoint dt = FixedPoint::createFromFloat(0.0166666f);
	FixedPoint halfDt = FixedPoint::createFromFloat(0.0166666f / 2.f);
	auto halfCalc = dt * half;
	auto dtf = dt.toFloat();
	auto halfDtf = halfDt.toFloat();
	auto halfCalcf = halfCalc.toFloat();

	auto root = sqrtF2F(FixedPoint::createFromFloat(-10.f));
	auto rootf = root.toFloat();

	return 1;
}