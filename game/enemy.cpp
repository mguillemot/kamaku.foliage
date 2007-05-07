#include <cmath>
#include <list>
#include <iostream>
#include "enemy.hpp"
#include "game_globals.hpp"
#include "bullet.hpp"

Foliage::Surface *Enemy::surface_ship[16];
Foliage::Surface *Enemy::surface_turret[16];

Enemy::Enemy(const GameColor c)
{
	_color = c;
	_killedDuration = 0;
	_life = 100;
	/*
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
	*/
	_sprite = new Foliage::Sprite(surface_ship[0]);
	for (int i = 1; i <= 15; i++)
	{
		_sprite->addFrame(surface_ship[i]);
	}
	_sprite->setDelayBetweenFrames(3);
	_hitbox[0] = Foliage::Rect(5, 17, 11, 32);
	_hitbox[1] = Foliage::Rect(12, 48, 37, 11);
	_hitbox[2] = Foliage::Rect(47, 17, 11, 32);
	//const Foliage::Rect hitbox(4, 13, 30, 15);
    //_sprite->setHitbox(hitbox);
	_turret = new Foliage::Sprite(surface_turret[0]);
	updateTurret();
}

Enemy::~Enemy()
{
    delete _sprite;
}

void Enemy::updateTurret()
{
	Foliage::Screen::drawLine(_sprite->getCenter(), currentLevel.playerShip->getCenter(), Foliage::Colors::White);
	Foliage::Fixed angleToPlayer = Foliage::Point::angleBetween(_sprite->getCenter(), currentLevel.playerShip->getCenter());
	angleToPlayer *= Sint16(8);
	angleToPlayer /= F_PI;
	Sint32 cran = -Sint32(angleToPlayer);
	if (cran < 0)
	{
		cran += 16;
	}
	_turret->changeFrame(surface_turret[cran]);
    _turretPosition = _sprite->getCenter(); //TEMP
}

void Enemy::loadSurfaces()
{
	surface_ship[0] = Foliage::BitmapLoader::loadBitmap("cewt01.bmp");
	surface_ship[1] = Foliage::BitmapLoader::loadBitmap("cewt02.bmp");
	surface_ship[2] = Foliage::BitmapLoader::loadBitmap("cewt03.bmp");
	surface_ship[3] = Foliage::BitmapLoader::loadBitmap("cewt04.bmp");
	surface_ship[4] = Foliage::BitmapLoader::loadBitmap("cewt05.bmp");
	surface_ship[5] = Foliage::BitmapLoader::loadBitmap("cewt06.bmp");
	surface_ship[6] = Foliage::BitmapLoader::loadBitmap("cewt07.bmp");
	surface_ship[7] = Foliage::BitmapLoader::loadBitmap("cewt08.bmp");
	surface_ship[8] = Foliage::BitmapLoader::loadBitmap("cewt09.bmp");
	surface_ship[9] = Foliage::BitmapLoader::loadBitmap("cewt10.bmp");
	surface_ship[10] = Foliage::BitmapLoader::loadBitmap("cewt11.bmp");
	surface_ship[11] = Foliage::BitmapLoader::loadBitmap("cewt12.bmp");
	surface_ship[12] = Foliage::BitmapLoader::loadBitmap("cewt13.bmp");
	surface_ship[13] = Foliage::BitmapLoader::loadBitmap("cewt14.bmp");
	surface_ship[14] = Foliage::BitmapLoader::loadBitmap("cewt15.bmp");
	surface_ship[15] = Foliage::BitmapLoader::loadBitmap("cewt16.bmp");
	surface_turret[0] = Foliage::BitmapLoader::loadBitmap("cewt_t0.bmp");
	surface_turret[1] = Foliage::BitmapLoader::loadBitmap("cewt_t1.bmp");
	surface_turret[2] = Foliage::BitmapLoader::loadBitmap("cewt_t2.bmp");
	surface_turret[3] = Foliage::BitmapLoader::loadBitmap("cewt_t3.bmp");
	surface_turret[4] = Foliage::BitmapLoader::loadBitmap("cewt_t4.bmp");
	surface_turret[5] = Foliage::BitmapLoader::loadBitmap("cewt_t5.bmp");
	surface_turret[6] = Foliage::BitmapLoader::loadBitmap("cewt_t6.bmp");
	surface_turret[7] = Foliage::BitmapLoader::loadBitmap("cewt_t7.bmp");
	surface_turret[8] = Foliage::BitmapLoader::loadBitmap("cewt_t8.bmp");
	surface_turret[9] = Foliage::BitmapLoader::loadBitmap("cewt_t9.bmp");
	surface_turret[10] = Foliage::BitmapLoader::loadBitmap("cewt_ta.bmp");
	surface_turret[11] = Foliage::BitmapLoader::loadBitmap("cewt_tb.bmp");
	surface_turret[12] = Foliage::BitmapLoader::loadBitmap("cewt_tc.bmp");
	surface_turret[13] = Foliage::BitmapLoader::loadBitmap("cewt_td.bmp");
	surface_turret[14] = Foliage::BitmapLoader::loadBitmap("cewt_te.bmp");
	surface_turret[15] = Foliage::BitmapLoader::loadBitmap("cewt_tf.bmp");
}

bool Enemy::isDead() const
{ 
	return (_life <= 0);
}

bool Enemy::hasDisappeared() const
{
	return (_killedDuration >= 10 || _sprite->outOfScreen());
}

void Enemy::setPosition(const Foliage::Point p)
{
	_sprite->setPosition(p);
	_turret->setPosition(p);
}

void Enemy::setSpeed(const Foliage::Speed s)
{
	_sprite->setSpeed(s);
	_turret->setSpeed(s);
}

bool Enemy::collidesWith(const Bullet *b)
{
	bool collides = false;
	const Foliage::Rect bHb = b->getSprite()->getScreenHitbox();
	const Foliage::Point pos = _sprite->getPosition();
	for (int i = 0; i < 3; i++)
	{
		Foliage::Rect h = _hitbox[i];
		h.shift(pos);
		if (Foliage::Rect::intersects(h, bHb))
		{
			collides = true;
			break;
		}
	}
	if (collides)
	{
		_life -= 25;
	}
	return collides;
}

void Enemy::drawHitbox(const Foliage::Color color) const
{
	const Foliage::Point pos = _sprite->getPosition();
	for (int i = 0; i < 3; i++)
	{
		Foliage::Rect r = _hitbox[i];
		r.shift(pos);
		Foliage::Screen::fillRect(r, color);
	}
}

void Enemy::display() const
{
	_sprite->draw();
	_turret->draw();
}


Bullet *Enemy::fireAt(const Foliage::Point p, const Foliage::Fixed speed, const Foliage::Fixed shift_angle, const BulletType bulletType) const
{
	const Foliage::Point pos = _sprite->getPosition();
	//const Foliage::Point from(pos.x + _turretPosition.x, pos.y + _turretPosition.y); //TEMP
	const Foliage::Point from = _turretPosition;
	const Foliage::Fixed angle = Foliage::Point::angleBetween(from, p) + shift_angle;
    Bullet *shot = new BulletGenerator(from, angle, speed, bulletType);
    currentLevel.enemyBullets.push_back(shot);
    return shot;
}

void Enemy::explode()
{
    _life = 0;
}

void Enemy::update()
{
	updateTurret();
	_sprite->move();
	_turret->move();
    if (isDead())
    {
        _killedDuration++;
    }
	else if (currentGame.frame % 50 == 0)
    {
		fireAt(currentLevel.playerShip->getCenter(), Foliage::Fixed(Sint16(2)), F_0, Bullet_Green_0);
		//fireAt(currentLevel.playerShip->getCenter(), Foliage::Fixed(Sint16(10)), F_0, Bullet_Standard);
		//fireAt(currentLevel.playerShip->getCenter(), Foliage::Fixed(Sint16(10)), Foliage::Fixed(0.2f), Bullet_Standard);
		//fireAt(currentLevel.playerShip->getCenter(), Foliage::Fixed(Sint16(10)), Foliage::Fixed(-0.2f), Bullet_Standard);
    }
}
