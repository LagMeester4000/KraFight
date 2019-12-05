#pragma once
#include <stdint.h>

/*
* This library aims to be an optimized implementation of fixed point math
* that still has an interface like a regular number.
*
* The reason I built this library is because I needed it in my online multiplayer
* fighting game. Fixed point math should produce consistent result on any machine,
* which is needed when building a game that needs to be deterministic.
* Floating points can sometimes behave strangely when performed on different
* machines or runtimes, or when the application is built by a different compiler.
* This library aims to make all of this consistent.
*
*
*/

namespace kra {
	// Compile time Pow function
	template<int A, int B>
	struct GetPower {
		const static int value = A * GetPower<A, B - 1>::value;
	};
	template<int A>
	struct GetPower<A, 0> {
		const static int value = 1;
	};

	// Get the type of the next size
	template<typename T>
	struct GetNext;
	template<>
	struct GetNext<int8_t> {
		using type = int16_t;
	};
	template<>
	struct GetNext<int16_t> {
		using type = int32_t;
	};
	template<>
	struct GetNext<int32_t> {
		using type = int64_t;
	};
	template<>
	struct GetNext<uint8_t> {
		using type = uint16_t;
	};
	template<>
	struct GetNext<uint16_t> {
		using type = uint32_t;
	};
	template<>
	struct GetNext<uint32_t> {
		using type = uint64_t;
	};

	// T = integer type
	// F = amount of fraction bits
	template<typename T, int F>
	class FixedPoint {
	public:
		using Type = T;
		using Next = typename GetNext<T>::type;
		const static int FractionBits = F;
		const static int FractionBitsPow = GetPower<2, FractionBits>::value;

	public:
		FixedPoint();
		FixedPoint(T val);
		FixedPoint(const FixedPoint<T, F>& other);

		//create a fixed point from a raw T value
		static FixedPoint<T, F> makeRaw(T rawValue);

		//create a fixed point from an integer that is converted to its 
		//corresponding fixed point value
		static FixedPoint<T, F> makeFromInt(T intValue);

		//create a fixed point from a fraction
		static FixedPoint<T, F> makeFromFraction(T a, T b);

		//compress back to integer type
		T toInt() const;

		//operators
		inline FixedPoint<T, F> operator+(const FixedPoint<T, F>& rhs) const;
		inline FixedPoint<T, F> operator-(const FixedPoint<T, F>& rhs) const;
		inline FixedPoint<T, F> operator*(const FixedPoint<T, F>& rhs) const;
		inline FixedPoint<T, F> operator/(const FixedPoint<T, F>& rhs) const;
		inline FixedPoint<T, F> operator%(const FixedPoint<T, F>& rhs) const;
		inline FixedPoint<T, F>& operator+=(const FixedPoint<T, F>& rhs);
		inline FixedPoint<T, F>& operator-=(const FixedPoint<T, F>& rhs);
		inline FixedPoint<T, F>& operator*=(const FixedPoint<T, F>& rhs);
		inline FixedPoint<T, F>& operator/=(const FixedPoint<T, F>& rhs);
		inline FixedPoint<T, F>& operator%=(const FixedPoint<T, F>& rhs);
		inline bool operator==(const FixedPoint<T, F>& rhs) const;
		inline bool operator!=(const FixedPoint<T, F>& rhs) const;
		inline bool operator<=(const FixedPoint<T, F>& rhs) const;
		inline bool operator<(const FixedPoint<T, F>& rhs) const;
		inline bool operator>=(const FixedPoint<T, F>& rhs) const;
		inline bool operator>(const FixedPoint<T, F>& rhs) const;
		inline operator bool() const;
		inline FixedPoint<T, F> operator-() const;
		inline FixedPoint<T, F>& operator=(const FixedPoint<T, F>& other);

		//consts
		//return an aproximation of pi
		static FixedPoint<T, F> pi();

	public: //members
		T value;
	};

	template<typename T, int F>
	inline FixedPoint<T, F>::FixedPoint()
	{
	}

	template<typename T, int F>
	inline FixedPoint<T, F>::FixedPoint(T val)
		: value(val)
	{
	}

	template<typename T, int F>
	inline FixedPoint<T, F>::FixedPoint(const FixedPoint<T, F>& other)
		: value(other.value)
	{
	}

	template<typename T, int F>
	inline FixedPoint<T, F> FixedPoint<T, F>::makeRaw(T rawValue)
	{
		return FixedPoint<T, F>(rawValue);
	}

	template<typename T, int F>
	inline FixedPoint<T, F> FixedPoint<T, F>::makeFromInt(T intValue)
	{
		//cross-compiler bitshift left
		return FixedPoint<T, F>(intValue * FractionBitsPow);
	}

	template<typename T, int F>
	inline FixedPoint<T, F> FixedPoint<T, F>::makeFromFraction(T a, T b)
	{
		return makeFromInt(a) / makeFromInt(b);
	}

	template<typename T, int F>
	inline T FixedPoint<T, F>::toInt() const
	{
		return value / FractionBitsPow;
	}

	template<typename T, int F>
	inline FixedPoint<T, F> FixedPoint<T, F>::operator+(const FixedPoint<T, F>& rhs) const
	{
		return FixedPoint<T, F>(value + rhs.value);
	}

	template<typename T, int F>
	inline FixedPoint<T, F> FixedPoint<T, F>::operator-(const FixedPoint<T, F>& rhs) const
	{
		return FixedPoint<T, F>(value - rhs.value);
	}

	template<typename T, int F>
	inline FixedPoint<T, F> FixedPoint<T, F>::operator*(const FixedPoint<T, F>& rhs) const
	{
		Next big = (Next)value * (Next)rhs.value;
		//cross-compiler bitshift right
		big /= FractionBitsPow;
		return FixedPoint<T, F>((Type)big);
	}

	template<typename T, int F>
	inline FixedPoint<T, F> FixedPoint<T, F>::operator/(const FixedPoint<T, F>& rhs) const
	{
		//cross-compiler bitshift left
		Next big = (Next)value * FractionBitsPow;
		big /= rhs.value;
		return FixedPoint<T, F>((Type)big);
	}

	template<typename T, int F>
	inline FixedPoint<T, F> FixedPoint<T, F>::operator%(const FixedPoint<T, F>& rhs) const
	{
		return FixedPoint<T, F>(value % rhs.value);
	}

	template<typename T, int F>
	inline FixedPoint<T, F>& FixedPoint<T, F>::operator+=(const FixedPoint<T, F>& rhs)
	{
		value += rhs.value;
		return *this;
	}

	template<typename T, int F>
	inline FixedPoint<T, F>& FixedPoint<T, F>::operator-=(const FixedPoint<T, F>& rhs)
	{
		value -= rhs.value;
		return *this;
	}

	template<typename T, int F>
	inline FixedPoint<T, F>& FixedPoint<T, F>::operator*=(const FixedPoint<T, F>& rhs)
	{
		Next big = (Next)value * (Next)rhs.value;
		//cross-compiler bitshift right
		big /= FractionBitsPow;
		value = (Type)big;
		return *this;
	}

	template<typename T, int F>
	inline FixedPoint<T, F>& FixedPoint<T, F>::operator/=(const FixedPoint<T, F>& rhs)
	{
		//cross-compiler bitshift left
		Next big = (Next)value * FractionBitsPow;
		big /= rhs.value;
		value = (Type)big;
		return *this;
	}

	template<typename T, int F>
	inline FixedPoint<T, F>& FixedPoint<T, F>::operator%=(const FixedPoint<T, F>& rhs)
	{
		value %= rhs.value;
		return *this;
	}

	template<typename T, int F>
	inline bool FixedPoint<T, F>::operator==(const FixedPoint<T, F>& rhs) const
	{
		return value == rhs.value;
	}

	template<typename T, int F>
	inline bool FixedPoint<T, F>::operator!=(const FixedPoint<T, F>& rhs) const
	{
		return value != rhs.value;
	}

	template<typename T, int F>
	inline bool FixedPoint<T, F>::operator<=(const FixedPoint<T, F>& rhs) const
	{
		return value <= rhs.value;
	}

	template<typename T, int F>
	inline bool FixedPoint<T, F>::operator<(const FixedPoint<T, F>& rhs) const
	{
		return value < rhs.value;
	}

	template<typename T, int F>
	inline bool FixedPoint<T, F>::operator>=(const FixedPoint<T, F>& rhs) const
	{
		return value >= rhs.value;
	}

	template<typename T, int F>
	inline bool FixedPoint<T, F>::operator>(const FixedPoint<T, F>& rhs) const
	{
		return value > rhs.value;
	}

	template<typename T, int F>
	inline FixedPoint<T, F>::operator bool() const
	{
		return value != (T)0;
	}

	template<typename T, int F>
	inline FixedPoint<T, F> FixedPoint<T, F>::operator-() const
	{
		return FixedPoint<T, F>(-value);
	}

	template<typename T, int F>
	inline FixedPoint<T, F>& FixedPoint<T, F>::operator=(const FixedPoint<T, F>& other)
	{
		value = other.value;
		return *this;
	}

	template<typename T, int F>
	inline FixedPoint<T, F> FixedPoint<T, F>::pi()
	{
		using Type = FixedPoint<T, F>;
		return Type::makeFromInt(355) / Type::makeFromInt(113);
	}

	template<typename U, typename T, int F>
	inline U toFloat(FixedPoint<T, F> value)
	{
		return (U)value.value / (U)FixedPoint<T, F>::FractionBitsPow;
	}

	template<typename T, int F>
	inline FixedPoint<T, F> floor(FixedPoint<T, F> value)
	{
		value /= FixedPoint<T, F>::FractionBitsPow;
		value *= FixedPoint<T, F>::FractionBitsPow;
		return value;
	}

	template<typename T, int F>
	inline FixedPoint<T, F> ceil(FixedPoint<T, F> value)
	{
		value = floor(value);
		value += FixedPoint<T, F>::makeFromInt(1);
		return value;
	}

	template<typename T, int F>
	inline FixedPoint<T, F> abs(FixedPoint<T, F> value)
	{
		return value.value < 0 ? FixedPoint<T, F>(-value.value) : value;
	}

	template<typename T, int F>
	inline FixedPoint<T, F> sin(FixedPoint<T, F> n)
	{
		using Fix = FixedPoint<T, F>;
		using FixNext = typename Fix::Next;
		using NextT = typename Fix::Type;
		const static Fix pi = Fix::pi();
		const static Fix half = Fix::makeFromInt(1) / Fix::makeFromInt(2);
		const static Fix halfPi = pi * half;
		const static Fix twoPi = pi * Fix::makeFromInt(2);

		//make negative input work
		Fix yMul = n.value >= 0 ? Fix::makeFromInt(1) : Fix::makeFromInt(-1);
		n = abs(n);

		//convert to radian
		n = (n * pi) / Fix::makeFromInt(180);

		//calculate negative
		n.value = n.value % twoPi.value;
		if (n.value >= pi.value)
		{
			yMul *= Fix::makeFromInt(-1);
			n -= pi;
		}

		//downwards curve
		if (n.value % pi.value > halfPi.value) // oxox
			n.value = halfPi.value - n.value % halfPi.value;

		//calculate input
		Fix z = n / halfPi;

		//pre-calculations
		Fix zSquare = z * z;

		//actual formula
		Fix ret = half * z * (pi - zSquare * ((Fix::makeFromInt(2) * pi - Fix::makeFromInt(5)) - zSquare * (pi - Fix::makeFromInt(3))));
		return ret * yMul;
	}

	template<typename T, int F>
	inline FixedPoint<T, F> cos(FixedPoint<T, F> n)
	{
		using Fix = FixedPoint<T, F>;
		return sin(n + Fix::makeFromInt(90));
	}

	template<typename T, int F>
	inline FixedPoint<T, F> sqrt(FixedPoint<T, F> n)
	{
		using Fix = FixedPoint<T, F>;
		using FixNext = typename Fix::Next;
		using NextT = typename Fix::Type;
		const static int precision = 10;
		const static Fix two = Fix::makeFromInt(2);

		//babylonian estimate
		//Fix e = n / two;
		Fix e = Fix::makeFromInt(1);
		for (int i = 0; i < precision; ++i)
		{
			e = (e + (n / e)) / two;
		}

		return e;
	}

	template<typename T, int F>
	inline FixedPoint<T, F> sqrtPrecise(FixedPoint<T, F> n, const int precision = 20)
	{
		using Fix = FixedPoint<T, F>;
		using FixNext = typename Fix::Next;
		using NextT = typename Fix::Type;
		const static Fix two = Fix::makeFromInt(2);

		//babylonian estimate
		Fix e = Fix::makeFromInt(1);
		for (int i = 0; i < precision; ++i)
		{
			e = (e + (n / e)) / two;
		}

		return e;
	}
}