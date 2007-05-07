#ifndef __ENEMY
#define __ENEMY

#include <list>
#include "bullet.hpp"
#include "foliage.hpp"
#include "gamecolor.hpp"

class Enemy
{
public:
    Enemy(const GameColor c = None);
    virtual     ~Enemy();

    virtual void update();
	Bullet      *fireAt(const Foliage::Point p, const Foliage::Fixed speed, const Foliage::Fixed shift_angle, const BulletType bulletType) const;
	virtual void display() const;
    void         explode();
	bool         isDead() const;
	bool         hasDisappeared() const;
	virtual bool collidesWith(const Bullet *b);
	virtual void drawHitbox(const Foliage::Color color) const;
	GameColor    getColor() const { return _color; }
	Foliage::Size getSize() const { return _sprite->getSize(); }
	virtual void setPosition(const Foliage::Point p);
	virtual void setSpeed(const Foliage::Speed s);

	static void loadSurfaces();

protected:
	Foliage::Point   _turretPosition;
	Foliage::Sprite *_sprite;
	Foliage::Sprite *_turret;
    Sint32           _killedDuration;
    Sint32           _life;
	GameColor        _color;
	Foliage::Rect    _hitbox[3];//TBH

	void         updateTurret();

	static Foliage::Surface *surface_ship[16]; //TO BE HERITED
	static Foliage::Surface *surface_turret[16]; //TO BE HERITED
};

typedef std::list<Enemy *> ListEnemy;

#endif // __ENEMY
