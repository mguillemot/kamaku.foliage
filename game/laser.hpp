#ifndef __LASER
#define __LASER

#include <list>
#include "foliage.hpp"

class Laser
{
public:
	Laser(const Foliage::Point generator, const Foliage::Fixed direction)
		: _generator(generator), _direction(direction)
	{
		_cos_direction = Foliage::FastMath::cos(direction);
		_sin_direction = Foliage::FastMath::sin(direction);
	}

	bool collidesWith(const Foliage::Rect &hitbox)
	{
		Foliage::Fixed j(_generator.y);
		Foliage::Fixed distance(Sint16(hitbox.x - _generator.x));
		distance *= _sin_direction;
		j += distance;
		for (Sint16 i = hitbox.x; i < (hitbox.x + hitbox.w); ++i)
		{
			const Sint16 j_tronc = Sint16(j);
			if (j_tronc >= hitbox.y && j_tronc <= (hitbox.y + hitbox.h))
			{
				return true;
			}
			j += _sin_direction;
		}
		return false;
	}

	void draw()
	{
		Foliage::Fixed distance_x(Foliage::Screen::Diagonal);
		distance_x *= _cos_direction;
		Foliage::Fixed distance_y(Foliage::Screen::Diagonal);
		distance_y *= _sin_direction;
		Foliage::Point outPoint;
		outPoint.x = _generator.x + Sint16(distance_x);
		outPoint.y = _generator.y + Sint16(distance_y);
		Foliage::Screen::drawLine(_generator, outPoint, Foliage::Colors::White);
	}

private:
	Foliage::Point _generator;
	Foliage::Fixed _direction;
	Foliage::Fixed _sin_direction, _cos_direction;
};

typedef std::list<Laser *> ListLaser;

#endif //__LASER
