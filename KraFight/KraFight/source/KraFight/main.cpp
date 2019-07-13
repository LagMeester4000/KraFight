#include "KraFight/Math/FixedPoint.h"

using namespace kra;

FixedPoint sqrtF2F(FixedPoint x)
{
	if (x.Value < 0)
	{
		return FixedPoint(-1);
	}

	uint32_t t, q, b, r;
	r = x.Value;
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

int main()
{
	FixedPoint half = FixedPoint::CreateFromFloat(0.5f);

	FixedPoint fp = FixedPoint::CreateFromFloat(2.5f);
	FixedPoint fp2 = FixedPoint::CreateFromFloat(-6.6f);
	auto res = fp2 / fp;
	auto f = res.toFloat();

	FixedPoint dt = FixedPoint::CreateFromFloat(0.0166666f);
	FixedPoint halfDt = FixedPoint::CreateFromFloat(0.0166666f / 2.f);
	auto halfCalc = dt * half;
	auto dtf = dt.toFloat();
	auto halfDtf = halfDt.toFloat();
	auto halfCalcf = halfCalc.toFloat();

	auto root = sqrtF2F(FixedPoint::CreateFromFloat(-10.f));
	auto rootf = root.toFloat();

	return 1;
}