#include <cmath>
#include <list>
#include <iostream>
#include "enemy.hpp"
#include "rythm_display.hpp"
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
	return (_killedDuration >= 10 || _entity->outOfScreen());
}

void Enemy::setPosition(const Foliage::Point p)
{
	_entity->setPosition(p);
}

void Enemy::setSpeed(const Foliage::Speed s)
{
	_entity->setSpeed(s);
}

void Enemy::display() const
{
	_entity->draw();
}

bool Enemy::collisionTest(Bullet *b)
{
	return Entity::collisionTest(_entity, b->getEntity());
}

void Enemy::drawHitbox(const Foliage::Color color) const
{
	_entity->drawHitbox(color);
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
	_entity->update();
    if (isDead())
    {
        _killedDuration++;
    }
}

///// DONUT ENEMY /////

Foliage::Surface         *DonutEnemy::surface_ship[16];
Foliage::RotativeSurface *DonutEnemy::rotativeSurface_turret;
Foliage::Point            DonutEnemy::turret_position[16];

void DonutEnemy::loadSurfaces()
{
	surface_ship[12] = Foliage::BmpLoader::loadBmp("cewt13.bmp");
	surface_ship[13] = Foliage::BmpLoader::loadBmp("cewt14.bmp");
	surface_ship[14] = Foliage::BmpLoader::loadBmp("cewt15.bmp");
	surface_ship[15] = Foliage::BmpLoader::loadBmp("cewt16.bmp");
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
	rotativeSurface_turret->setSurface(Foliage::Direction(0), Foliage::BmpLoader::loadBmp("cewt_t0.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(1), Foliage::BmpLoader::loadBmp("cewt_t1.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(2), Foliage::BmpLoader::loadBmp("cewt_t2.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(3), Foliage::BmpLoader::loadBmp("cewt_t3.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(4), Foliage::BmpLoader::loadBmp("cewt_t4.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(5), Foliage::BmpLoader::loadBmp("cewt_t5.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(6), Foliage::BmpLoader::loadBmp("cewt_t6.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(7), Foliage::BmpLoader::loadBmp("cewt_t7.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(8), Foliage::BmpLoader::loadBmp("cewt_t8.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(9), Foliage::BmpLoader::loadBmp("cewt_t9.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(10), Foliage::BmpLoader::loadBmp("cewt_ta.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(11), Foliage::BmpLoader::loadBmp("cewt_tb.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(12), Foliage::BmpLoader::loadBmp("cewt_tc.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(13), Foliage::BmpLoader::loadBmp("cewt_td.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(14), Foliage::BmpLoader::loadBmp("cewt_te.bmp"));
	rotativeSurface_turret->setSurface(Foliage::Direction(15), Foliage::BmpLoader::loadBmp("cewt_tf.bmp"));

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
	AnimatedEntity *s = new AnimatedEntity(16);
	for (int i = 0; i <= 15; i++)
	{
		s->addFrame(surface_ship[i]);
	}
	s->setDelayBetweenFrames(3);
	s->getHitbox()->addRect(Foliage::Rect(5, 17, 11, 32));
	s->getHitbox()->addRect(Foliage::Rect(12, 48, 37, 11));
	s->getHitbox()->addRect(Foliage::Rect(47, 17, 11, 32));
	_entity = s;
	_turret = new RotativeEntity(rotativeSurface_turret);
	_cran = -1;
	updateTurret();
}

DonutEnemy::~DonutEnemy()
{
    delete _entity;
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
	Sint16 dx = currentLevel->playerShip->getCenter().x - _entity->getCenter().x;
	Sint16 dy = currentLevel->playerShip->getCenter().y - _entity->getCenter().y;
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
	if (!isDead() && (currentLevel->rythm->tap(0)))
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
	surface_ship[12] = Foliage::BmpLoader::loadBmp("flyerc.bmp");
	surface_ship[13] = Foliage::BmpLoader::loadBmp("flyerd.bmp");
	surface_ship[14] = Foliage::BmpLoader::loadBmp("flyere.bmp");
	surface_ship[15] = Foliage::BmpLoader::loadBmp("flyerf.bmp");
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

FlyerEnemy::FlyerEnemy() : _trajectory(92)
{
	_life = 65;
	_entity = new RotativeEntity(rot_surface_ship);
	_entity->getHitbox()->addRect(Foliage::Rect(13, 11, 21, 26));
	//_turretPosition = Foliage::Point(24, 24);

	// Trajectory
	Trajectory &traj = _trajectory;
	traj.setStep(0, Foliage::Speed(Foliage::Fixed(0.0f), Foliage::Fixed(0.0f)));
	traj.setStep(1, Foliage::Speed(Foliage::Fixed(-0.5993195f), Foliage::Fixed(4.853184f)));
	traj.setStep(2, Foliage::Speed(Foliage::Fixed(-0.5693665f), Foliage::Fixed(4.880466f)));
	traj.setStep(3, Foliage::Speed(Foliage::Fixed(-0.5561218f), Foliage::Fixed(4.900187f)));
	traj.setStep(4, Foliage::Speed(Foliage::Fixed(-0.5523376f), Foliage::Fixed(4.914184f)));
	traj.setStep(5, Foliage::Speed(Foliage::Fixed(-0.5550995f), Foliage::Fixed(4.925687f)));
	traj.setStep(6, Foliage::Speed(Foliage::Fixed(-0.5625f), Foliage::Fixed(4.932753f)));
	traj.setStep(7, Foliage::Speed(Foliage::Fixed(-0.5743561f), Foliage::Fixed(4.940976f)));
	traj.setStep(8, Foliage::Speed(Foliage::Fixed(-0.589798f), Foliage::Fixed(4.946171f)));
	traj.setStep(9, Foliage::Speed(Foliage::Fixed(-0.6090546f), Foliage::Fixed(4.951131f)));
	traj.setStep(10, Foliage::Speed(Foliage::Fixed(-0.6322021f), Foliage::Fixed(4.955311f)));
	traj.setStep(11, Foliage::Speed(Foliage::Fixed(-0.6596222f), Foliage::Fixed(4.95849f)));
	traj.setStep(12, Foliage::Speed(Foliage::Fixed(-0.6918945f), Foliage::Fixed(4.960825f)));
	traj.setStep(13, Foliage::Speed(Foliage::Fixed(-0.7300415f), Foliage::Fixed(4.9629f)));
	traj.setStep(14, Foliage::Speed(Foliage::Fixed(-0.7751617f), Foliage::Fixed(4.963238f)));
	traj.setStep(15, Foliage::Speed(Foliage::Fixed(-0.8292542f), Foliage::Fixed(4.962132f)));
	traj.setStep(16, Foliage::Speed(Foliage::Fixed(-0.8953094f), Foliage::Fixed(4.960327f)));
	traj.setStep(17, Foliage::Speed(Foliage::Fixed(-0.9774628f), Foliage::Fixed(4.955059f)));
	traj.setStep(18, Foliage::Speed(Foliage::Fixed(-1.08287f), Foliage::Fixed(4.944923f)));
	traj.setStep(19, Foliage::Speed(Foliage::Fixed(-1.224136f), Foliage::Fixed(4.92617f)));
	traj.setStep(20, Foliage::Speed(Foliage::Fixed(-3.137985f), Foliage::Fixed(10.45712f)));
	traj.setStep(21, Foliage::Speed(Foliage::Fixed(-1.600266f), Foliage::Fixed(4.761192f)));
	traj.setStep(22, Foliage::Speed(Foliage::Fixed(-1.708069f), Foliage::Fixed(4.722549f)));
	traj.setStep(23, Foliage::Speed(Foliage::Fixed(-1.827362f), Foliage::Fixed(4.67733f)));
	traj.setStep(24, Foliage::Speed(Foliage::Fixed(-1.958374f), Foliage::Fixed(4.62294f)));
	traj.setStep(25, Foliage::Speed(Foliage::Fixed(-2.1017f), Foliage::Fixed(4.558136f)));
	traj.setStep(26, Foliage::Speed(Foliage::Fixed(-2.257568f), Foliage::Fixed(4.481453f)));
	traj.setStep(27, Foliage::Speed(Foliage::Fixed(-2.425598f), Foliage::Fixed(4.390999f)));
	traj.setStep(28, Foliage::Speed(Foliage::Fixed(-2.604553f), Foliage::Fixed(4.284752f)));
	traj.setStep(29, Foliage::Speed(Foliage::Fixed(-2.792725f), Foliage::Fixed(4.161682f)));
	traj.setStep(30, Foliage::Speed(Foliage::Fixed(-2.987457f), Foliage::Fixed(4.020966f)));
	traj.setStep(31, Foliage::Speed(Foliage::Fixed(-3.185089f), Foliage::Fixed(3.862587f)));
	traj.setStep(32, Foliage::Speed(Foliage::Fixed(-3.381454f), Foliage::Fixed(3.687706f)));
	traj.setStep(33, Foliage::Speed(Foliage::Fixed(-4.070358f), Foliage::Fixed(3.842461f)));
	traj.setStep(34, Foliage::Speed(Foliage::Fixed(-3.917862f), Foliage::Fixed(3.085831f)));
	traj.setStep(35, Foliage::Speed(Foliage::Fixed(-4.129211f), Foliage::Fixed(2.802216f)));
	traj.setStep(36, Foliage::Speed(Foliage::Fixed(-4.293015f), Foliage::Fixed(2.551605f)));
	traj.setStep(37, Foliage::Speed(Foliage::Fixed(-4.421982f), Foliage::Fixed(2.330673f)));
	traj.setStep(38, Foliage::Speed(Foliage::Fixed(-4.525146f), Foliage::Fixed(2.135361f)));
	traj.setStep(39, Foliage::Speed(Foliage::Fixed(-4.609009f), Foliage::Fixed(1.96196f)));
	traj.setStep(40, Foliage::Speed(Foliage::Fixed(-4.678726f), Foliage::Fixed(1.807373f)));
	traj.setStep(41, Foliage::Speed(Foliage::Fixed(-4.737991f), Foliage::Fixed(1.668991f)));
	traj.setStep(42, Foliage::Speed(Foliage::Fixed(-4.789169f), Foliage::Fixed(1.5448f)));
	traj.setStep(43, Foliage::Speed(Foliage::Fixed(-4.834991f), Foliage::Fixed(1.433945f)));
	traj.setStep(44, Foliage::Speed(Foliage::Fixed(-4.87973f), Foliage::Fixed(1.337891f)));
	traj.setStep(45, Foliage::Speed(Foliage::Fixed(-10.36977f), Foliage::Fixed(2.075378f)));
	traj.setStep(46, Foliage::Speed(Foliage::Fixed(-5.0252f), Foliage::Fixed(0.1615601f)));
	traj.setStep(47, Foliage::Speed(Foliage::Fixed(-4.951927f), Foliage::Fixed(-0.6098022f)));
	traj.setStep(48, Foliage::Speed(Foliage::Fixed(-4.810242f), Foliage::Fixed(-1.209824f)));
	traj.setStep(49, Foliage::Speed(Foliage::Fixed(-4.708282f), Foliage::Fixed(-1.506577f)));
	traj.setStep(50, Foliage::Speed(Foliage::Fixed(-9.77037f), Foliage::Fixed(-3.347717f)));
	traj.setStep(51, Foliage::Speed(Foliage::Fixed(-4.681038f), Foliage::Fixed(-1.621094f)));
	traj.setStep(52, Foliage::Speed(Foliage::Fixed(-4.675255f), Foliage::Fixed(-1.677124f)));
	traj.setStep(53, Foliage::Speed(Foliage::Fixed(-4.657074f), Foliage::Fixed(-1.757446f)));
	traj.setStep(54, Foliage::Speed(Foliage::Fixed(-4.626869f), Foliage::Fixed(-1.859558f)));
	traj.setStep(55, Foliage::Speed(Foliage::Fixed(-4.583435f), Foliage::Fixed(-1.984344f)));
	traj.setStep(56, Foliage::Speed(Foliage::Fixed(-4.523308f), Foliage::Fixed(-2.134842f)));
	traj.setStep(57, Foliage::Speed(Foliage::Fixed(-4.440945f), Foliage::Fixed(-2.316223f)));
	traj.setStep(58, Foliage::Speed(Foliage::Fixed(-4.326645f), Foliage::Fixed(-2.535873f)));
	traj.setStep(59, Foliage::Speed(Foliage::Fixed(-4.164837f), Foliage::Fixed(-2.803894f)));
	traj.setStep(60, Foliage::Speed(Foliage::Fixed(-3.929016f), Foliage::Fixed(-3.132401f)));
	traj.setStep(61, Foliage::Speed(Foliage::Fixed(-7.057922f), Foliage::Fixed(-7.879684f)));
	traj.setStep(62, Foliage::Speed(Foliage::Fixed(-2.719196f), Foliage::Fixed(-4.204468f)));
	traj.setStep(63, Foliage::Speed(Foliage::Fixed(-2.311481f), Foliage::Fixed(-4.428192f)));
	traj.setStep(64, Foliage::Speed(Foliage::Fixed(-1.938229f), Foliage::Fixed(-4.592598f)));
	traj.setStep(65, Foliage::Speed(Foliage::Fixed(-1.630566f), Foliage::Fixed(-4.702454f)));
	traj.setStep(66, Foliage::Speed(Foliage::Fixed(-1.399441f), Foliage::Fixed(-4.770439f)));
	traj.setStep(67, Foliage::Speed(Foliage::Fixed(-1.240688f), Foliage::Fixed(-4.810349f)));
	traj.setStep(68, Foliage::Speed(Foliage::Fixed(-1.142509f), Foliage::Fixed(-4.831505f)));
	traj.setStep(69, Foliage::Speed(Foliage::Fixed(-1.09201f), Foliage::Fixed(-4.841377f)));
	traj.setStep(70, Foliage::Speed(Foliage::Fixed(-1.077175f), Foliage::Fixed(-4.843063f)));
	traj.setStep(71, Foliage::Speed(Foliage::Fixed(-1.08864f), Foliage::Fixed(-4.840164f)));
	traj.setStep(72, Foliage::Speed(Foliage::Fixed(-1.283257f), Foliage::Fixed(-5.577507f)));
	traj.setStep(73, Foliage::Speed(Foliage::Fixed(-1.033661f), Foliage::Fixed(-4.806049f)));
	traj.setStep(74, Foliage::Speed(Foliage::Fixed(-0.9675732f), Foliage::Fixed(-4.836723f)));
	traj.setStep(75, Foliage::Speed(Foliage::Fixed(-0.9193726f), Foliage::Fixed(-4.860245f)));
	traj.setStep(76, Foliage::Speed(Foliage::Fixed(-0.8818607f), Foliage::Fixed(-4.877186f)));
	traj.setStep(77, Foliage::Speed(Foliage::Fixed(-0.8522072f), Foliage::Fixed(-4.893482f)));
	traj.setStep(78, Foliage::Speed(Foliage::Fixed(-0.8275013f), Foliage::Fixed(-4.905766f)));
	traj.setStep(79, Foliage::Speed(Foliage::Fixed(-0.8069191f), Foliage::Fixed(-4.917698f)));
	traj.setStep(80, Foliage::Speed(Foliage::Fixed(-0.7892246f), Foliage::Fixed(-4.927696f)));
	traj.setStep(81, Foliage::Speed(Foliage::Fixed(-0.7740383f), Foliage::Fixed(-4.93749f)));
	traj.setStep(82, Foliage::Speed(Foliage::Fixed(-0.7608299f), Foliage::Fixed(-4.946798f)));
	traj.setStep(83, Foliage::Speed(Foliage::Fixed(-0.7492638f), Foliage::Fixed(-4.955613f)));
	traj.setStep(84, Foliage::Speed(Foliage::Fixed(-0.7393456f), Foliage::Fixed(-4.965631f)));
	traj.setStep(85, Foliage::Speed(Foliage::Fixed(-0.7305412f), Foliage::Fixed(-4.974375f)));
	traj.setStep(86, Foliage::Speed(Foliage::Fixed(-0.7232943f), Foliage::Fixed(-4.985494f)));
	traj.setStep(87, Foliage::Speed(Foliage::Fixed(-0.7171593f), Foliage::Fixed(-4.996068f)));
	traj.setStep(88, Foliage::Speed(Foliage::Fixed(-0.7127495f), Foliage::Fixed(-5.00971f)));
	traj.setStep(89, Foliage::Speed(Foliage::Fixed(-0.7102413f), Foliage::Fixed(-5.025688f)));
	traj.setStep(90, Foliage::Speed(Foliage::Fixed(-0.7108116f), Foliage::Fixed(-5.047319f)));
	traj.setStep(91, Foliage::Speed(Foliage::Fixed(-0.7171736f), Foliage::Fixed(-5.080114f)));
}

FlyerEnemy::~FlyerEnemy()
{
	delete _entity;
}

void FlyerEnemy::update()
{
	setSpeed(_trajectory.nextStep());
	if (_trajectory.ended())
	{
		explode();
	}
	Enemy::update();
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
