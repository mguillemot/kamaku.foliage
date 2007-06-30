#ifndef __ENTITY
#define __ENTITY

#include <vector>
#include "foliage.hpp"
#include "hitbox.hpp"

class Entity
{
public:	
	Entity()
	{
		_constrained = false;
	}

	virtual ~Entity()
	{
	}
	
	virtual Foliage::Surface* getCurrentSurface() = 0;

	virtual void setSpeed(const Foliage::Speed s)
	{
		_speed = s;
	}

	Foliage::Speed getSpeed() const
	{
		return _speed;
	}

	void setConstrained(bool constr)
	{
		_constrained = constr;
	}

	bool outOfScreen() const
	{
		const Foliage::Point pos = getPosition();
		const Foliage::Size size = getSize();
		return (pos.y >= Foliage::Screen::Height || (pos.y + size.h) <= 0
			|| pos.x >= Foliage::Screen::Width || (pos.x + size.w) <= 0);
	}

	virtual void update()
	{
		_x += _speed.x;
		_y += _speed.y;
		if (_constrained)
		{
			const Foliage::Point pos = getPosition();
			for (std::vector<Foliage::Rect>::const_iterator i = _hitbox.rects().begin(); i != _hitbox.rects().end(); ++i)
			{
				Foliage::Rect r = *i;
				r.shift(pos);
				if (r.x < 0)
				{
					_x = Foliage::Fixed(Sint16(- i->x));
				}
				else if ((r.x + r.w) >= Foliage::Screen::Width)
				{
					_x = Foliage::Fixed(Sint16(Foliage::Screen::Width - i->x - r.w));
				}
				if (r.y < 0)
				{
					_y = Foliage::Fixed(Sint16(- i->y));
				}
				else if ((r.y + r.h) >= Foliage::Screen::Height)
				{
					_y = Foliage::Fixed(Sint16(Foliage::Screen::Height - i->y - r.h));
				}
			}				
		}
	}

	void draw()
	{
		getCurrentSurface()->drawAt(getPosition());
	}

	void setPosition(const Foliage::Point p)
	{
		_x = Foliage::Fixed(p.x);
		_y = Foliage::Fixed(p.y);
	}

	Foliage::Point getPosition() const
	{
		return Foliage::Point(Sint16(_x), Sint16(_y));
	}

	Foliage::Point getCenter() const
	{
		const Foliage::Size s = getSize();
		Foliage::Point pos = getPosition();
		pos.x += s.w >> 1;
		pos.y += s.h >> 1;
		return pos;
	}

	Hitbox* getHitbox()
	{ 
		return &_hitbox; 
	}

	Foliage::Size getSize() const
	{
		return _size;
	}

	void drawHitbox(const Foliage::Color color) const
	{
		_hitbox.drawCollidableZoneAt(color, getPosition());
	}

	static bool collisionTest(Entity* a, Entity* b)
	{
		return Hitbox::collisionTest(a->getPosition(), a->getHitbox(), b->getPosition(), b->getHitbox());
	}

protected:
	Foliage::Size   _size;
	Foliage::Fixed  _x, _y;
	Foliage::Speed  _speed;
	Hitbox          _hitbox;
	bool            _constrained;
};

class SimpleEntity : public Entity
{
public:
	SimpleEntity(Foliage::Surface* surface)
		: _surface(surface)
	{
		_size = surface->getSize();
	}

	virtual ~SimpleEntity()
	{
	}

	void changeFrame(Foliage::Surface* surface)
	{
		_surface = surface;
	}

	virtual Foliage::Surface* getCurrentSurface()
	{
		return _surface;
	}

protected:
	Foliage::Surface* _surface;
};

class RotativeEntity : public Entity
{
public:
	RotativeEntity(Foliage::RotativeSurface* rSurface)
		: _rSurface(rSurface)
	{
		_size = _rSurface->getSize();
		_direction = Foliage::DIR_S;
	}

	virtual ~RotativeEntity()
	{
	}

	virtual Foliage::Surface* getCurrentSurface()
	{
		return _rSurface->surface(_direction);
	}

	virtual void setSpeed(const Foliage::Speed s)
	{
		Entity::setSpeed(s);
		_direction = Foliage::FastMath::direction(s.x, s.y);
	}

	void setDirection(const Foliage::Direction dir)
	{
		_direction = dir;
	}

protected:
	Foliage::RotativeSurface* _rSurface;
	Foliage::Direction        _direction;
};

class AnimatedEntity : public Entity
{
public:
	AnimatedEntity(Sint32 nbFrames)
		: _animation(nbFrames)
	{
	}

	virtual ~AnimatedEntity()
	{
	}

	virtual Foliage::Surface* getCurrentSurface()
	{
		return _animation.current();
	}

	void addFrame(Foliage::Surface* surface)
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
		Entity::update();
		_delayCount--;
		if (_delayCount <= 0)
		{
			_delayCount = _delayBetweenFrames;
			_animation.next();
		}
	}

protected:
	Foliage::SimpleAnimation _animation;
	Sint32                   _delayBetweenFrames, _delayCount;
};

class AnimatedRotativeSprite : public Entity
{
	//TODO
private: AnimatedRotativeSprite(){}
};

#endif //__ENTITY
