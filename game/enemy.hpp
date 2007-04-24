#ifndef __ENEMY
#define __ENEMY

#include <list>
#include "foliage.hpp"
#include "bullet.hpp"
#include "gamecolor.hpp"

class Enemy
{
public:
    Enemy(const GameColor c = None);
    virtual ~Enemy();
    virtual void update(); // = 0
	Bullet      *fireAt(const Foliage::Point p, const Foliage::Fixed speed, const Foliage::Fixed shift_angle, const BulletType bulletType) const;
	Foliage::Sprite *getSprite() const { return _sprite; }
    void         explode();
	bool         getKilled() const { return _killed; }
	int          getKilledTime() const { return _killedTime; }
	bool         collidesWith(const Bullet *b) const;
    virtual void damage(const Sint32 damage);
	GameColor    getColor() const { return _color; }

protected:
	Foliage::Point   _turret;
	Foliage::Sprite *_sprite;
    bool             _killed;
    Uint32           _killedTime;
    Sint32           _life;
	GameColor        _color;
};

typedef std::list<Enemy *> ListEnemy;

#endif // __ENEMY
