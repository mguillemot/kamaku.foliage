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
#define	FLOAT_RESOLUTIONf 0 
//#define FLOAT_RESOLUTIONf 0.0000152587890625f

#define F_0            Foliage::Fixed(true, 0)
#define F_1            Foliage::Fixed(true, 1 << 16)
#define F_10           Foliage::Fixed(true, 10 << 16)
#define F_100          Foliage::Fixed(true, 100 << 16)
#define F_1000         Foliage::Fixed(true, 1000 << 16)
#define F_10000        Foliage::Fixed(true, 10000 << 16)
#define F_0_DOT_1      Foliage::Fixed(true, 6553)
#define F_PI           Foliage::Fixed(true, 205887)
#define F_PI_2         Foliage::Fixed(true, 102943)
#define F_3_PI_2       Foliage::Fixed(true, 308830)
#define F_TWOPI        Foliage::Fixed(true, 411774)
#define F_2000_OVER_PI Foliage::Fixed(true, 41721513)
#define F_MINUS_PI_2   Foliage::Fixed(true, -205887)

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

	class Fixed
	{
	public:
		Fixed() : v(0) { }
		Fixed(const Fixed &fix) : v(fix.v) { }
		explicit Fixed(const Sint16 i) { v = i << 16; }
		explicit Fixed(const float f) { v = (Sint32)((f + FLOAT_RESOLUTIONf) * FIXED_RESOLUTIONf); }
		Fixed(const bool internal, const Sint32 v) : v(v) { }
		~Fixed() { }
		
		Fixed & operator=(const Fixed &fix) { v = fix.v; return *this; }
		Fixed & operator=(const Sint16 i) { v = i << 16; return *this; }
		Fixed & operator=(const float f) { v = (Sint32)std::floor((f + FLOAT_RESOLUTIONf) * FIXED_RESOLUTIONf); return *this; }

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
		
		const Fixed opposite() const { return Fixed(true, -v); }
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
		
		friend std::ostream & operator<<(std::ostream &s, const Fixed &fix);
	
	private:
		Sint32 v;
	};

}

#endif //_FOLIAGE__BASIC_TYPES
