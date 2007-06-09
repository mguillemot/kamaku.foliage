#ifndef __FOLIAGE_SPRITE
#define __FOLIAGE_SPRITE

#include <string>
#include <list>
#include "../basic_types.hpp"
#include "hitbox.hpp"
#include "graphic_types.hpp"
#include "animation.hpp"
#include "rotative_surface.hpp"

namespace Foliage
{
	class Sprite
	{
	public:	
		// Constructor & destructor
		Sprite()
		{
			_constrained = false;
		}

		virtual ~Sprite()
		{
		}
		
		// Instance methods
		virtual Surface *getCurrentSurface() = 0;

		virtual void setSpeed(const Speed s)
		{
			_speed = s;
		}

		Speed getSpeed() const
		{
			return _speed;
		}

		void setConstrained(bool constr)
		{
			_constrained = constr;
		}

		virtual void update()
		{
			_x += _speed.x;
			_y += _speed.y;
			if (_constrained)
			{
				const Point pos = getPosition();
				if (pos.x < 0)
				{
					_x = F_0;
				}
				else if ((pos.x + _size.w) >= Screen::Width)
				{
					_x = Fixed(Sint16(Screen::Width - _size.w));
				}
				if (pos.y < 0)
				{
					_y = F_0;
				}
				else if ((pos.y + _size.h) >= Screen::Height)
				{
					_y = Fixed(Sint16(Screen::Height - _size.h));
				}
			}
		}

		void draw()
		{
			getCurrentSurface()->drawAt(getPosition());
		}

		void setPosition(const Point p)
		{
			_x = Foliage::Fixed(p.x);
			_y = Foliage::Fixed(p.y);
		}

		Point getPosition() const
		{
			return Foliage::Point(Sint16(_x), Sint16(_y));
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

		Hitbox *getHitbox()
		{ 
			return &_hitbox; 
		}

		Size getSize() const
		{
			return _size;
		}

		void drawHitbox(const Color color) const
		{
			_hitbox.drawCollidableZoneAt(color, getPosition());
		}

		static bool collisionTest(Sprite *a, Sprite *b)
		{
			return Hitbox::collisionTest(a->getPosition(), a->getHitbox(), b->getPosition(), b->getHitbox());
		}

	protected:
		Size   _size;
		Fixed  _x, _y;
		Speed  _speed;
		Hitbox _hitbox;
		bool   _constrained;
	};

	class SimpleSprite : public Sprite
	{
	public:
		SimpleSprite(Surface *surface)
			: _surface(surface)
		{
			_size = surface->getSize();
		}

		virtual ~SimpleSprite()
		{
		}

		void changeFrame(Surface *surface)
		{
			_surface = surface;
		}

		virtual Surface *getCurrentSurface()
		{
			return _surface;
		}

	protected:
		Surface *_surface;
	};

	class RotativeSprite : public Sprite
	{
	public:
		RotativeSprite(RotativeSurface *rSurface)
			: _rSurface(rSurface)
		{
			_size = _rSurface->getSize();
		}

		virtual ~RotativeSprite()
		{
		}

		virtual Surface *getCurrentSurface()
		{
			return _rSurface->surface(_direction);
		}

		virtual void setSpeed(const Speed s)
		{
			Sprite::setSpeed(s);
			_direction = Foliage::FastMath::direction(s.x, s.y);
		}

		void setDirection(const Direction dir)
		{
			_direction = dir;
		}

	protected:
		RotativeSurface *_rSurface;
		Direction        _direction;
	};

	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite(Sint32 nbFrames)
			: _animation(nbFrames)
		{
		}

		virtual ~AnimatedSprite()
		{
		}

		virtual Surface *getCurrentSurface()
		{
			return _animation.current();
		}

		void addFrame(Surface *surface)
		{
			_animation.add(surface);
			_size = surface->getSize();
		}

		void setDelayBetweenFrames(const Sint32 delay)
		{
			_delayBetweenFrames = delay;
			_delayCount = delay;
		}

		virtual void update()
		{
			Sprite::update();
			_delayCount--;
			if (_delayCount <= 0)
			{
				_delayCount = _delayBetweenFrames;
				_animation.next();
			}
		}

	protected:
		SimpleAnimation _animation;
		Sint32          _delayBetweenFrames, _delayCount;
	};

	class AnimatedRotativeSprite : public Sprite
	{

	};
}

#endif //__FOLIAGE_SPRITE
