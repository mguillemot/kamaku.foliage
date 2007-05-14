#include <iostream>
#include "bullet.hpp"
#include "fastmath.hpp"
#include "game_globals.hpp"

#define NB_BULLET_TYPES 6

std::vector<Foliage::Surface *> Bullet::_bulletSurfaces(NB_BULLET_TYPES);

void Bullet::loadBulletSurfaces()
{
	_bulletSurfaces[0] = Foliage::BitmapLoader::loadBitmap("bul.bmp");
	Foliage::Instancizator::instancize(_bulletSurfaces[0]);
	_bulletSurfaces[1] = Foliage::BitmapLoader::loadBitmap("bul_red.bmp");
	Foliage::Instancizator::instancize(_bulletSurfaces[1]);
	_bulletSurfaces[2] = Foliage::BitmapLoader::loadBitmap("bul_blue.bmp");
	Foliage::Instancizator::instancize(_bulletSurfaces[2]);
	_bulletSurfaces[3] = Foliage::BitmapLoader::loadBitmap("bul_gree.bmp");
	Foliage::Instancizator::instancize(_bulletSurfaces[3]);
	_bulletSurfaces[4] = Foliage::BitmapLoader::loadBitmap("bullet0.bmp");
	Foliage::Instancizator::instancize(_bulletSurfaces[4]);
	_bulletSurfaces[5] = Foliage::BitmapLoader::loadBitmap("bullet1.bmp");
	Foliage::Instancizator::instancize(_bulletSurfaces[5]);
}

Bullet::Bullet(const Foliage::Point position, const Foliage::Fixed direction, const Foliage::Fixed speed, const BulletType type)
	: _direction(direction)
{
	_dead = false;
    _sprite = new Foliage::Sprite(_bulletSurfaces[type]);
    const Foliage::Size size = _sprite->getSize();
	if (type >= 0 && type <= 3)
	{
		// enemy round bullet (8x8)
		const Foliage::Rect bulletHitbox = Foliage::Rect(2, 2, 4, 4);
		_sprite->setHitbox(bulletHitbox);
	}
	else
	{
		// my digital bullets (6x9)
		const Foliage::Rect bulletHitbox = Foliage::Rect(0, 0, 6, 9);
		_sprite->setHitbox(bulletHitbox);
	}    
    _sprite->setPosition(Foliage::Point(position.x - (size.w >> 1), position.y - (size.h >> 1)));
    setSpeed(speed);
}

Bullet::~Bullet()
{
    delete _sprite;
}

void Bullet::updateSpriteSpeed()
{
    const Foliage::Fixed sx = _speed * Foliage::FastMath::cos(_direction);
    const Foliage::Fixed sy = _speed * Foliage::FastMath::sin(_direction);
    _sprite->setSpeed(Foliage::Speed(sx, sy));
}

void Bullet::update()
{
    _sprite->move();
}

void Bullet::setSpeed(const Foliage::Fixed speed)
{
    _speed = speed;
    updateSpriteSpeed();
}

void Bullet::setDirection(const Foliage::Fixed direction)
{
    _direction = direction;
    updateSpriteSpeed();
}

void BulletGenerator::update()
{
	Bullet::update();
	_generateTimer--;
	if (_generateTimer == 0)
	{
		_dead = true;
		const Foliage::Point from = _sprite->getCenter();
		const int N = 20;
		Foliage::Fixed space = F_TWOPI;
		space /= Sint16(N);
		Foliage::Fixed angle;
		const Foliage::Fixed speed(2.5f);
		for (int i = 0; i < N; i++)
		{
			Bullet *shot = new Bullet(from, angle, speed, Bullet_Standard);
			currentLevel->enemyBullets.push_back(shot);
			angle += space;
		}
	}
}
