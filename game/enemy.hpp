#ifndef __ENEMY
#define __ENEMY

#include <list>
#include "bullet.hpp"
#include "foliage.hpp"
#include "gamecolor.hpp"

class Enemy
{
public:
                 Enemy();
    virtual     ~Enemy();

    virtual void   update();
	Bullet        *fireAt(const Foliage::Point turret, const Foliage::Point target, const Foliage::Fixed speed, const Foliage::Fixed shift_angle, const BulletType bulletType) const;
	virtual void   display() const;
    void           explode();
	bool           isDead() const;
	bool           hasDisappeared() const;
	bool           collisionTest(Bullet *b);
	void           drawHitbox(const Foliage::Color color) const;
	Foliage::Size  getSize() const { return _sprite->getSize(); }
	virtual void   setPosition(const Foliage::Point p);
	Foliage::Point getPosition() const { return _sprite->getPosition(); }
	virtual void   setSpeed(const Foliage::Speed s);
	Foliage::Sprite *getSprite() { return _sprite; }

protected:
	Foliage::Sprite *_sprite;
    Sint32           _killedDuration;
    Sint32           _life;
};

typedef std::list<Enemy *> ListEnemy;

class ColoredEnemy : public Enemy
{
public:
	static void loadSurfaces();

	         ColoredEnemy(const GameColor c);
	virtual ~ColoredEnemy();

protected:
	GameColor _color;
};

class DonutEnemy : public Enemy
{
public:
	static void loadSurfaces();

	         DonutEnemy();
	virtual ~DonutEnemy();

	virtual void display() const;
	virtual void setPosition(const Foliage::Point p);
	virtual void setSpeed(const Foliage::Speed s);
	virtual void update();

protected:
	static Foliage::Surface         *surface_ship[16];
	static Foliage::RotativeSurface *rotativeSurface_turret;
	static Foliage::Point            turret_position[16];

	void             updateTurret();

	Foliage::RotativeSprite *_turret;
	Sint16                   _cran;
};

class FlyerEnemy : public Enemy
{
public:
	static void loadSurfaces();

	         FlyerEnemy();
	virtual ~FlyerEnemy();

protected:
	static Foliage::RotativeSurface *rot_surface_ship;
};

class TankEnemy : public Enemy
{
public:
	static void loadSurfaces();

	         TankEnemy();
	virtual ~TankEnemy();

protected:
};

#endif // __ENEMY
