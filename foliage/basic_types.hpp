#ifndef _FOLIAGE__BASIC_TYPES
#define _FOLIAGE__BASIC_TYPES

#include <ostream>
#include <cmath>

#ifdef __PPC__
	#include <xbasic_types.h>
	typedef Xuint8   Uint8;
	typedef Xint8    Sint8;
	typedef Xuint16  Uint16;
	typedef Xint16   Sint16;
	typedef Xuint32  Uint32;
	typedef Xint32   Sint32;
#else
	#include <SDL.h>
#endif

#define FIXED_RESOLUTION  65536
#define FIXED_RESOLUTIONf 65536.0f

#define F_0                 Foliage::Fixed(true, 0)
#define F_1                 Foliage::Fixed(true, 1 << 16)
#define F_10                Foliage::Fixed(true, 10 << 16)
#define F_100               Foliage::Fixed(true, 100 << 16)
#define F_1000              Foliage::Fixed(true, 1000 << 16)
#define F_10000             Foliage::Fixed(true, 10000 << 16)
#define F_0_DOT_1           Foliage::Fixed(true, 6553)
#define F_PI                Foliage::Fixed(true, 205887)
#define F_PI_2              Foliage::Fixed(true, 102943)
#define F_3_PI_2            Foliage::Fixed(true, 308830)
#define F_TWOPI             Foliage::Fixed(true, 411774)
#define F_2000_OVER_PI      Foliage::Fixed(true, 41721513)
#define F_8_OVER_PI         Foliage::Fixed(true, 166886)
#define F_MINUS_PI_2        Foliage::Fixed(true, -205887)
#define F_TAN_PI_OVER_16    Foliage::Fixed(true, 13036)
#define F_TAN_3_PI_OVER_16  Foliage::Fixed(true, 43790)
#define F_TAN_5_PI_OVER_16  Foliage::Fixed(true, 98082)
#define F_TAN_7_PI_OVER_16  Foliage::Fixed(true, 329472)

inline Sint32 mul32x32(Sint32 a, Sint32 b)
{
	long long int ll = (long long int)a * (long long int)b;
	long long int llm = ll >> 16;
	return (Sint32)llm;
}

inline Sint32 div32x32(Sint32 a, Sint32 b)
{
	long long int ll = (long long int)a << 16;
	ll /= b;
	return (Sint32)ll;
}

namespace Foliage
{
	enum Direction
	{
		DIR_E = 0,
		DIR_ENE = 1,
		DIR_NE = 2,
		DIR_NNE = 3,
		DIR_N = 4,
		DIR_NNW = 5,
		DIR_NW = 6,
		DIR_WNW = 7,
		DIR_W = 8,
		DIR_WSW = 9,
		DIR_SW = 10,
		DIR_SSW = 11,
		DIR_S = 12,
		DIR_SSE = 13,
		DIR_SE = 14,
		DIR_ESE = 15
	};

	class Fixed
	{
	public:
		Fixed() : v(0) { }
		Fixed(const Fixed &fix) : v(fix.v) { }
		explicit Fixed(const Sint16 i) { v = i << 16; }
		explicit Fixed(const float f) { v = (Sint32)std::floor(f * FIXED_RESOLUTIONf); }
		Fixed(const bool internal, const Sint32 v) : v(v) { }
		~Fixed() { }
		
		Fixed & operator=(const Fixed &fix) { v = fix.v; return *this; }
		Fixed & operator=(const Sint16 i) { v = i << 16; return *this; }
		Fixed & operator=(const float f) { v = (Sint32)std::floor(f * FIXED_RESOLUTIONf); return *this; }

		Fixed & operator+=(const Fixed &fix) { v += fix.v; return *this; }
		Fixed & operator+=(const Sint16 i) { v += i << 16; return *this; }
		Fixed & operator-=(const Fixed &fix) { v -= fix.v; return *this; }
		Fixed & operator-=(const Sint16 i) { v -= i << 16; return *this; }
		Fixed & operator*=(const Fixed &fix) { v = mul32x32(v, fix.v); return *this; }
		Fixed & operator*=(const Sint16 i)	{ v *= i; return *this; }
		Fixed & operator/=(const Fixed &fix) { v = div32x32(v, fix.v); return *this; }
		Fixed & operator/=(const Sint16 i)	{ v /= i; return *this; }
		
		const Fixed operator+(const Fixed &fix) const { return Fixed(*this) += fix; }
		const Fixed operator+(const Sint16 i) const { return Fixed(*this) += i; }
		const Fixed operator-(const Fixed &fix) const { return Fixed(*this) -= fix; }
		const Fixed operator-(const Sint16 i) const { return Fixed(*this) -= i; }
		const Fixed operator*(const Fixed &fix) const { return Fixed(*this) *= fix; }
		const Fixed operator*(const Sint16 i) const { return Fixed(*this) *= i; }
		const Fixed operator/(const Fixed &fix) const { return Fixed(*this) /= fix; }
		const Fixed operator/(const Sint16 i) const { return Fixed(*this) /= i; }
		
		const Fixed opposite() const 
		{ 
			return Fixed(true, -v); 
		}

		const Fixed copy() const
		{
			return Fixed(true, v);
		}

		const Sint16 round() const
		{ 
			Uint32 firstDecimal = (v & (1 << 15));
			if (firstDecimal != 0)
			{
				return (v >> 16) + 1;
			}
			else
			{
				return (v >> 16);
			}
		}

		Direction angleToDirection() const
		{
			Foliage::Fixed theta = copy();
			theta.normalizeAngle();
			theta *= F_8_OVER_PI;
			Sint16 dir = theta.round();
			if (dir > 15)
			{
				dir -= 16;
			}
			else if (dir < 0)
			{
				dir += 16;
			}
			return Direction(dir);
		}

		void normalizeAngle()
		{
			// Angles are in the [0;2*PI[ interval
			while (v > 411774)
			{
				v -= 411774;
			}
			while (v <= 0)
			{
				v += 411774;
			}
		}
		
		bool operator<(const Fixed &fix) const { return (v < fix.v); }
		bool operator<(const Sint16 i) const { return (v < (i << 16)); }
		bool operator>(const Fixed &fix) const { return (v > fix.v); }
		bool operator>(const Sint16 i) const { return (v > (i << 16)); }
		bool operator<=(const Fixed &fix) const { return (v <= fix.v); }
		bool operator<=(const Sint16 i) const { return (v <= (i << 16)); }
		bool operator>=(const Fixed &fix) const { return (v >= fix.v); }
		bool operator>=(const Sint16 i) const { return (v >= (i << 16)); }
		bool operator==(const Fixed &fix) const { return (v == fix.v); }
		bool operator==(const Sint16 i) const { return (v == (i << 16)); }
		bool operator!=(const Fixed &fix) const { return (v != fix.v); }
		bool operator!=(const Sint16 i) const { return (v != (i << 16)); }

		operator Sint16() const { return (v >> 16); }
		operator float() const { return (float(v) / FIXED_RESOLUTIONf); }
		
		friend std::ostream & operator<<(std::ostream &s, const Fixed &fix)
		{
			return (s << float(fix));
		}
	
	private:
		Sint32 v;
	};
}

#endif //_FOLIAGE__BASIC_TYPES
