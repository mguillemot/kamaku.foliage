#ifndef __FOLIAGE_SPRITE
#define __FOLIAGE_SPRITE

#include "../basic_types.hpp"
#include "graphic_types.hpp"

namespace Foliage
{
	class Sprite
	{
	public:	
		void draw() const
		{
			_surface->drawAt(_position);
		}

		Point getPosition() const
		{
			return _position;
		}

		void setPosition(const Point pos)
		{
			_position = pos;
		}

		Surface* getSurface()
		{
			return _surface;
		}

		void setSurface(Surface* surface)
		{
			_surface = surface;
		}

		void move(Sint16 dx, Sint16 dy)
		{
			_position.x += dx;
			_position.y += dy;
		}

		Point getCenter() const
		{
			const Foliage::Size s = getSize();
			Foliage::Point pos = getPosition();
			pos.x += s.w >> 1;
			pos.y += s.h >> 1;
			return pos;
		}

		bool outOfScreen() const
		{
			const Foliage::Point pos = getPosition();
			const Foliage::Size size = getSize();
			return (pos.y >= Foliage::Screen::Height || (pos.y + size.h) <= 0
				|| pos.x >= Foliage::Screen::Width || (pos.x + size.w) <= 0);
		}

		Size getSize() const
		{
			return _surface->getSize();
		}

	protected:
		Point    _position;
		Surface* _surface; 
	};
}

#endif //__FOLIAGE_SPRITE
