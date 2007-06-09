#include <cmath>
#include <list>
#include <iostream>
#include "enemy.hpp"
#include "game_globals.hpp"
#include "bullet.hpp"

///// BASE CLASS /////

Enemy::Enemy()
{
	_killedDuration = 0;
	_life = 100;
}

Enemy::~Enemy()
{
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
}

void Enemy::setSpeed(const Foliage::Speed s)
{
	_sprite->setSpeed(s);
}

void Enemy::display() const
{
	_sprite->draw();
}

bool Enemy::collisionTest(Bullet *b)
{
	return Foliage::Sprite::collisionTest(_sprite, b->getSprite());
}

void Enemy::drawHitbox(const Foliage::Color color) const
{
	_sprite->drawHitbox(color);
}

Bullet *Enemy::fireAt(const Foliage::Point turret, const Foliage::Point target, const Foliage::Fixed speed, const Foliage::Fixed shift_angle, const BulletType bulletType) const
{
	Foliage::Fixed angle = Foliage::Point::angleBetween(turret, target) + shift_angle;
	angle.normalizeAngle();
    Bullet *shot = new BulletGenerator(turret, angle, speed, bulletType);
    currentLevel->enemyBullets.push_back(shot);
    return shot;
}

void Enemy::explode()
{
    _life = 0;
}

void Enemy::update()
{
	_sprite->update();
    if (isDead())
    {
        _killedDuration++;
    }
}

///// COLORED ENEMY /////

void ColoredEnemy::loadSurfaces()
{
}

ColoredEnemy::ColoredEnemy(const GameColor c)
{
	_life = 50;
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
	_color = c;
	//_sprite = new Foliage::Sprite(surface_ship[0]);
	//_sprite->setDelayBetweenFrames(3);
	const Foliage::Rect hitbox(4, 13, 30, 15);
	_sprite->getHitbox()->addRect(hitbox);
}

ColoredEnemy::~ColoredEnemy()
{
    delete _sprite;
}

///// DONUT ENEMY /////

Foliage::Surface         *DonutEnemy::surface_ship[16];
Foliage::RotativeSurface *DonutEnemy::rotativeSurface_turret;
Foliage::Point            DonutEnemy::turret_position[16];

void DonutEnemy::loadSurfaces()
{
	surface_ship[12] = Foliage::BitmapLoader::loadBitmap("cewt13.bmp");
	surface_ship[13] = Foliage::BitmapLoader::loadBitmap("cewt14.bmp");
	surface_ship[14] = Foliage::BitmapLoader::loadBitmap("cewt15.bmp");
	surface_ship[15] = Foliage::BitmapLoader::loadBitmap("cewt16.bmp");
	surface_ship[0] = surface_ship[12]->createNewRotatedSurface(270);
	surface_ship[1] = surface_ship[13]->createNewRotatedSurface(270);
	surface_ship[2] = surface_ship[14]->createNewRotatedSurface(270);
	surface_ship[3] = surface_ship[15]->createNewRotatedSurface(270);
	surface_ship[4] = surface_ship[12]->createNewRotatedSurface(180);
	surface_ship[5] = surface_ship[13]->createNewRotatedSurface(180);
	surface_ship[6] = surface_ship[14]->createNewRotatedSurface(180);
	surface_ship[7] = surface_ship[15]->createNewRotatedSurface(180);
	surface_ship[8] = surface_ship[12]->createNewRotatedSurface(90);
	surface_ship[9] = surface_ship[13]->createNewRotatedSurface(90);
	surface_ship[10] = surface_ship[14]->createNewRotatedSurface(90);
	surface_ship[11] = surface_ship[15]->createNewRotatedSurface(90);

	rotativeSurface_turret = new Foliage::RotativeSurface();
	rotativeSurface_turret->setSurface(Foliage::Direction(0), Foliage::BitmapLoader::loadBitmap("cewt_t0.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(1), Foliage::BitmapLoader::loadBitmap("cewt_t1.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(2), Foliage::BitmapLoader::loadBitmap("cewt_t2.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(3), Foliage::BitmapLoader::loadBitmap("cewt_t3.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(4), Foliage::BitmapLoader::loadBitmap("cewt_t4.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(5), Foliage::BitmapLoader::loadBitmap("cewt_t5.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(6), Foliage::BitmapLoader::loadBitmap("cewt_t6.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(7), Foliage::BitmapLoader::loadBitmap("cewt_t7.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(8), Foliage::BitmapLoader::loadBitmap("cewt_t8.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(9), Foliage::BitmapLoader::loadBitmap("cewt_t9.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(10), Foliage::BitmapLoader::loadBitmap("cewt_ta.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(11), Foliage::BitmapLoader::loadBitmap("cewt_tb.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(12), Foliage::BitmapLoader::loadBitmap("cewt_tc.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(13), Foliage::BitmapLoader::loadBitmap("cewt_td.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(14), Foliage::BitmapLoader::loadBitmap("cewt_te.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(15), Foliage::BitmapLoader::loadBitmap("cewt_tf.bmp"));

	turret_position[0] = Foliage::Point(59, 33);
	turret_position[1] = Foliage::Point(57, 22);
	turret_position[2] = Foliage::Point(52, 13);
	turret_position[3] = Foliage::Point(42, 7);
	turret_position[4] = Foliage::Point(31, 4);
	turret_position[5] = Foliage::Point(20, 6);
	turret_position[6] = Foliage::Point(11, 13);
	turret_position[7] = Foliage::Point(5, 23);
	turret_position[8] = Foliage::Point(4, 34);
	turret_position[9] = Foliage::Point(5, 44);
	turret_position[10] = Foliage::Point(10, 53);
	turret_position[11] = Foliage::Point(20, 58);
	turret_position[12] = Foliage::Point(32, 59);
	turret_position[13] = Foliage::Point(41, 58);
	turret_position[14] = Foliage::Point(50, 53);
	turret_position[15] = Foliage::Point(57, 44);
}

DonutEnemy::DonutEnemy()
{
	_life = 100;
	Foliage::AnimatedSprite *s = new Foliage::AnimatedSprite(16);
	for (int i = 0; i <= 15; i++)
	{
		s->addFrame(surface_ship[i]);
	}
	s->setDelayBetweenFrames(3);
	s->getHitbox()->addRect(Foliage::Rect(5, 17, 11, 32));
	s->getHitbox()->addRect(Foliage::Rect(12, 48, 37, 11));
	s->getHitbox()->addRect(Foliage::Rect(47, 17, 11, 32));
	_sprite = s;
	_turret = new Foliage::RotativeSprite(rotativeSurface_turret);
	_cran = -1;
	updateTurret();
}

DonutEnemy::~DonutEnemy()
{
    delete _sprite;
	delete _turret;
}

void DonutEnemy::display() const
{
	Enemy::display();
	_turret->draw();
}

void DonutEnemy::updateTurret()
{
	//Foliage::Fixed angleToPlayer = Foliage::Point::angleBetween(_sprite->getCenter(), currentLevel->playerShip->getCenter()).opposite();
	//Sint16 cran = angleToPlayer.angleToDirection();
	Sint16 dx = currentLevel->playerShip->getCenter().x - _sprite->getCenter().x;
	Sint16 dy = currentLevel->playerShip->getCenter().y - _sprite->getCenter().y;
	Foliage::Direction cran = Foliage::FastMath::direction(dx, dy);
	if (cran != _cran)
	{
		_turret->setDirection(cran);
		_cran = cran;
	}
}

void DonutEnemy::setPosition(const Foliage::Point p)
{
	Enemy::setPosition(p);
	_turret->setPosition(p);
}

void DonutEnemy::setSpeed(const Foliage::Speed s)
{
	Enemy::setSpeed(s);
	_turret->setSpeed(s);
}

void DonutEnemy::update()
{
	Enemy::update();
	updateTurret();
	_turret->update();
	if (!isDead() && currentGame->frame % 50 == 0)
    {
		Foliage::Point turret = getPosition() + turret_position[_cran];
		fireAt(turret, currentLevel->playerShip->getCenter(), Foliage::Fixed(Sint16(2)), F_0, Bullet_Green_0);
    }
}

///// FLYER ENEMY /////

Foliage::RotativeSurface *FlyerEnemy::rot_surface_ship;

void FlyerEnemy::loadSurfaces()
{
	Foliage::Surface *surface_ship[16];	
	surface_ship[12] = Foliage::BitmapLoader::loadBitmap("flyerc.bmp");
	surface_ship[13] = Foliage::BitmapLoader::loadBitmap("flyerd.bmp");
	surface_ship[14] = Foliage::BitmapLoader::loadBitmap("flyere.bmp");
	surface_ship[15] = Foliage::BitmapLoader::loadBitmap("flyerf.bmp");
	surface_ship[0] = surface_ship[12]->createNewRotatedSurface(270);
	surface_ship[1] = surface_ship[13]->createNewRotatedSurface(270);
	surface_ship[2] = surface_ship[14]->createNewRotatedSurface(270);
	surface_ship[3] = surface_ship[15]->createNewRotatedSurface(270);
	surface_ship[4] = surface_ship[12]->createNewRotatedSurface(180);
	surface_ship[5] = surface_ship[13]->createNewRotatedSurface(180);
	surface_ship[6] = surface_ship[14]->createNewRotatedSurface(180);
	surface_ship[7] = surface_ship[15]->createNewRotatedSurface(180);
	surface_ship[8] = surface_ship[12]->createNewRotatedSurface(90);
	surface_ship[9] = surface_ship[13]->createNewRotatedSurface(90);
	surface_ship[10] = surface_ship[14]->createNewRotatedSurface(90);
	surface_ship[11] = surface_ship[15]->createNewRotatedSurface(90);
	rot_surface_ship = new Foliage::RotativeSurface();
	for (int i = 0; i < 15; ++i)
	{
		rot_surface_ship->setSurface(Foliage::Direction(i), surface_ship[i]);
	}
}

FlyerEnemy::FlyerEnemy()
{
	_life = 65;
	_sprite = new Foliage::RotativeSprite(rot_surface_ship);
	_sprite->getHitbox()->addRect(Foliage::Rect(13, 11, 21, 26));
	//_turretPosition = Foliage::Point(24, 24);
}

FlyerEnemy::~FlyerEnemy()
{
	delete _sprite;
}

///// TANK ENEMY /////

void TankEnemy::loadSurfaces()
{
}

TankEnemy::TankEnemy()
{
}

TankEnemy::~TankEnemy()
{
}
