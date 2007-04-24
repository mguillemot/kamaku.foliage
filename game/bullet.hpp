#ifndef __BULLET
#define __BULLET

#include <list>
#include <vector>
#include "foliage.hpp"

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
    ~Bullet();

	// Instance methods
	Foliage::Sprite *getSprite() { return _sprite; }
    void             setSpeed(const Foliage::Fixed speed);
	Foliage::Fixed   getSpeed() const { return _speed; }
	void             setDirection(const Foliage::Fixed direction);
	Foliage::Fixed   getDirection() const { return _direction; }
	Sint32           getType() const { return _type; }
    void             update();

	// Static methods
	static void      loadBulletSurfaces();

private:
    void             updateSpriteSpeed();

    Foliage::Fixed   _direction;
    Foliage::Fixed   _speed;
    Foliage::Sprite *_sprite;
	Sint32           _type;
    
	static std::vector<Foliage::Surface *> _bulletSurfaces;
};

typedef std::list<Bullet *> ListBullet; //, PoolAllocator<Bullet*, LevelPool> (disabled for x86 compatibility)

#endif
