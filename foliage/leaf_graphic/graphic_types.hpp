#ifndef __FOLIAGE_GRAPHIC_TYPES
#define __FOLIAGE_GRAPHIC_TYPES

#include "../basic_types.hpp"
#include "../settings.hpp"
#include "../fastmath.hpp"

namespace Foliage
{

	typedef Uint16 Color;

	enum ScreenMode { YOKO, TATE };

	struct Size
	{
		Sint16 w, h;
		
		Size() : w(0), h(0)
		{
		}
		
		Size(const Sint16 sw, const Sint16 sh)
			: w(sw), h(sh)
		{
		}
	};
	
	struct Point
	{
		Sint16 x, y;
		
		Point() : x(0), y(0)
		{
		}
		
		Point(const Sint16 px, const Sint16 py)
			: x(px), y(py)
		{
		}

		static Fixed angleBetween(const Point from, const Point to)
		{
			const Sint16 dx = to.x - from.x;
			const Sint16 dy = to.y - from.y;
			Fixed angle;
			if (dx != 0)
			{
				angle = FastMath::atan(Foliage::Fixed(dy) / Foliage::Fixed(dx));
			}
			else if (dy > 0)
			{
				angle = F_PI_2;
			}
			else
			{
				angle = F_MINUS_PI_2;
			}
			if (dx < 0)
			{
				angle += F_PI;
			}
			return angle;
		}

		const Point operator+(const Point s) const
		{
			return Point(x + s.x, y + s.y);
		}
	};
	
	struct Speed
	{
		Fixed x, y;
		
		Speed() : x(), y()
		{
		}
		
		Speed(const Fixed &sx, const Fixed &sy)
			: x(sx), y(sy)
		{
		}
	};

	struct Rect
	{
		Sint16 x, y;
		Sint16 w, h;
		
		Rect() : x(0), y(0), w(0), h(0)
		{
		}
		
		Rect(const Sint16 rx, const Sint16 ry, const Sint16 rw, const Sint16 rh)
			: x(rx), y(ry), w(rw), h(rh)
		{
		}
		
		Rect(const Point p, const Size s)
			: x(p.x), y(p.y), w(s.w), h(s.h)
		{
		}

		void shift(const Point shift)
		{
			x += shift.x;
			y += shift.y;
		}

		Point center() const
		{
			Point p(x, y);
			p.x += w >> 1;
			p.y += h >> 1;
			return p;
		}
		
		static bool intersects(const Rect &a, const Rect &b);
	};

	inline bool Rect::intersects(const Foliage::Rect &a, const Foliage::Rect &b)
	{
		return !(b.x > a.x + a.w || b.x + b.w < a.x || b.y > a.y + a.h || b.y + b.h < a.y);
	}
	
}

#endif //__FOLIAGE_GRAPHIC_TYPES
