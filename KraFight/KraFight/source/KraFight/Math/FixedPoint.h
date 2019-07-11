#pragma once
#include <stdint.h>

namespace kra {
	template<int I>
	struct SizedType;
	template<>
	struct SizedType<1> {
		using type = int8_t;
	}; 
	template<>
	struct SizedType<2> {
		using type = int16_t;
	};
	template<>
	struct SizedType<3> {
		using type = int32_t;
	};
	template<>
	struct SizedType<4> {
		using type = int32_t;
	};
	template<>
	struct SizedType<5> {
		using type = int64_t;
	};
	template<>
	struct SizedType<6> {
		using type = int64_t;
	};
	template<>
	struct SizedType<7> {
		using type = int64_t;
	};
	template<>
	struct SizedType<8> {
		using type = int64_t;
	};
	template<int I>
	using SizeTypeT = typename SizedType<I>::type;

	//template<int I, int F>
	//class FixedPoint {
	//	using Type = SizeTypeT<(I + F - 1) / 8>;
	//public:
	//
	//
	//private:
	//	Type Val;
	//};
}

namespace kra {
	class FixedPoint
	{
	public:
		FixedPoint();
		FixedPoint(int32_t inValue);
		FixedPoint(const FixedPoint &otherFixedPoint);
		FixedPoint& operator=(int32_t intValue);
		FixedPoint& operator=(FixedPoint otherFixedPoint);
		FixedPoint operator+(FixedPoint otherFixedPoint) const;
		FixedPoint operator-(FixedPoint otherFixedPoint) const;
		FixedPoint operator*(FixedPoint otherFixedPoint) const;
		FixedPoint operator/(FixedPoint otherFixedPoint) const;
		static FixedPoint createFromInt(int32_t value);
		static FixedPoint createFromFloat(float value);
		float toFloat();
		FixedPoint Root(FixedPoint Value);

		int32_t value;
		static const int32_t FractionBits = 12;
		static const int32_t FirstIntegerBitSet = 1 << FractionBits;
	};
}