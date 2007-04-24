#include <cmath>
#include <list>
#include <iostream>
#include "enemy.hpp"
#include "bullet.hpp"

extern Foliage::Sprite *playerShip;
extern ListBullet enemyBullets;

Enemy::Enemy(const GameColor c)
{
	_color = c;
	_killed = false;
	_killedTime = 0;
	_life = 100;
	switch (c)
	{
	case Red:
		_sprite = new Foliage::Sprite("e_red.bmp");
		break;
	case Green:
		_sprite = new Foliage::Sprite("e_green.bmp");
		break;
	case Blue:
		_sprite = new Foliage::Sprite("e_blue.bmp");
		break;
	case Yellow:
		_sprite = new Foliage::Sprite("e_yellow.bmp");
		break;
	case Purple:
		_sprite = new Foliage::Sprite("e_purple.bmp");
		break;
	case Lightblue:
		_sprite = new Foliage::Sprite("e_lblue.bmp");
		break;
	case Black:
		_sprite = new Foliage::Sprite("e_black.bmp");
		break;
	case White:
		_sprite = new Foliage::Sprite("e_white.bmp");
		break;
	default:
		_sprite = new Foliage::Sprite("ennemi.bmp");
		break;
	}
	const Foliage::Rect hitbox(4, 13, 30, 15);
    _sprite->setHitbox(hitbox);
    _turret = _sprite->getCenter();
}

Enemy::~Enemy()
{
    delete _sprite;
}

Bullet *Enemy::fireAt(const Foliage::Point p, const Foliage::Fixed speed, const Foliage::Fixed shift_angle, const BulletType bulletType) const
{
	const Foliage::Point pos = _sprite->getPosition();
	const Foliage::Point from(pos.x + _turret.x, pos.y + _turret.y);
	Foliage::Fixed angle;
    if (p.x < 0 && p.y < 0)
    {
        angle = shift_angle;
    }
    else
    {
        const Sint16 dx = p.x - from.x;
        const Sint16 dy = p.y - from.y;
        if (dx != 0)
        {
			angle = Foliage::FastMath::atan(Foliage::Fixed(dy) / Foliage::Fixed(dx));
        }
        else if (dy > 0)
        {
            angle = F_PI_2;
        }
        else
        {
            angle = F_MINUS_PI_2;
        }
        if (dx < 0)
        {
            angle += F_PI;
        }
        angle += shift_angle;
    }
    Bullet *shot = new Bullet(from, angle, speed, bulletType);
    enemyBullets.push_back(shot);
    return shot;
}

void Enemy::explode()
{
    _killed = true;
}

void Enemy::damage(const Sint32 damage)
{
	_life -= damage;
    if (_life <= 0)
    {
        explode();
    }
}

void Enemy::update()
{
    if (_killed)
    {
        _killedTime++;
    }
	// return;
    if (!_killed)
    {
		//_fireCount++;
		//if (_fireCount == 5)
        {
            //_fireCount = 0;
            //float speed = 10.0f + (float)_difficulty / 5;
			//fireAt(playerShip->getCenter(), Foliage::Fixed(Sint16(10)), F_0, Bullet_Standard);
			//fireAt(playerShip->getCenter(), Foliage::Fixed(Sint16(10)), Foliage::Fixed(0.2f), Bullet_Standard);
			//fireAt(playerShip->getCenter(), Foliage::Fixed(Sint16(10)), Foliage::Fixed(-0.2f), Bullet_Standard);
        }
    }
}
