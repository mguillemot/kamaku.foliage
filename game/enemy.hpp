#ifndef __ENEMY
#define __ENEMY

#include <list>
#include "entity.hpp"
#include "bullet.hpp"
#include "foliage.hpp"
#include "trajectory.hpp"

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
	Foliage::Size  getSize() const { return _entity->getSize(); }
	virtual void   setPosition(const Foliage::Point p);
	Foliage::Point getPosition() const { return _entity->getPosition(); }
	virtual void   setSpeed(const Foliage::Speed s);
	Entity*        getEntity() { return _entity; }

protected:
	Entity* _entity;
    Sint32  _killedDuration;
    Sint32  _life;
};

typedef std::list<Enemy *> ListEnemy;

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

	RotativeEntity* _turret;
	Sint16          _cran;
};

class FlyerEnemy : public Enemy
{
public:
	static void loadSurfaces();

	         FlyerEnemy();
	virtual ~FlyerEnemy();

	virtual void update();

protected:
	static Foliage::RotativeSurface* rot_surface_ship;

	Trajectory _trajectory;
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
