#include <iostream>
#include "bullet.hpp"
#include "fastmath.hpp"

#define NB_BULLET_TYPES     4

std::vector<Foliage::Surface *> Bullet::_bulletSurfaces(NB_BULLET_TYPES);

void Bullet::loadBulletSurfaces()
{
	_bulletSurfaces[0] = Foliage::BitmapLoader::loadBitmap("bullet.bmp");
	Foliage::Instancizator::instancize(_bulletSurfaces[0]);
	_bulletSurfaces[1] = Foliage::BitmapLoader::loadBitmap("boulette_rouge.bmp");
	Foliage::Instancizator::instancize(_bulletSurfaces[1]);
	_bulletSurfaces[2] = Foliage::BitmapLoader::loadBitmap("boulette_bleue.bmp");
	Foliage::Instancizator::instancize(_bulletSurfaces[2]);
	_bulletSurfaces[3] = Foliage::BitmapLoader::loadBitmap("boulette_verte.bmp");
	Foliage::Instancizator::instancize(_bulletSurfaces[3]);
}

Bullet::Bullet(const Foliage::Point position, const Foliage::Fixed direction, const Foliage::Fixed speed, const Sint32 type)
	: _direction(direction)
{
    _sprite = new Foliage::Sprite(_bulletSurfaces[type]);
    const Foliage::Size size = _sprite->getSize();
    const Foliage::Rect bouletteHitbox = Foliage::Rect(2, 2, 4, 4);
    _sprite->setHitbox(bouletteHitbox);
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
