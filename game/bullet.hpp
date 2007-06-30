#ifndef __BULLET
#define __BULLET

#include <list>
#include <vector>
#include "foliage.hpp"
#include "entity.hpp"

enum BulletType
{ 
	Bullet_Standard = 0,
	Bullet_Red = 1,
	Bullet_Blue = 2,
	Bullet_Green = 3,
	Bullet_Green_0 = 4,
	Bullet_Green_1 = 5
};

class Bullet
{
public:
	// Constructor & destructor
    Bullet(const Foliage::Point position, const Foliage::Fixed direction, const Foliage::Fixed speed, const BulletType type);
    virtual ~Bullet();

	// Instance methods
	Entity*          getEntity() { return _entity; }
	const Entity*    getEntity() const { return _entity; }
    void             setSpeed(const Foliage::Fixed speed);
	Foliage::Fixed   getSpeed() const { return _speed; }
	void             setDirection(const Foliage::Fixed direction);
	Foliage::Fixed   getDirection() const { return _direction; }
	Sint32           getType() const { return _type; }
    virtual void     update();
	bool             isDead() const { return (_dead || _entity->outOfScreen()); }

	// Static methods
	static void      loadBulletSurfaces();

protected:
    void             updateEntitySpeed();

    Foliage::Fixed   _direction;
    Foliage::Fixed   _speed;
    Entity*          _entity;
	Sint32           _type;
	bool             _dead;
    
	static std::vector<Foliage::Surface *> _bulletSurfaces;
};

typedef std::list<Bullet *> ListBullet; //, PoolAllocator<Bullet*, LevelPool> (disabled for x86 compatibility)

class BulletGenerator : public Bullet
{
public:
	BulletGenerator(const Foliage::Point position, const Foliage::Fixed direction, const Foliage::Fixed speed, const BulletType type)
		: Bullet(position, direction, speed, type)
	{
		_generateTimer = 40;
	}

    virtual ~BulletGenerator()
	{
	}

	virtual void update();

protected:
	Sint32 _generateTimer;
};

#endif
