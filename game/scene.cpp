#include <iostream>
#include "scene.hpp"
#include "utils.hpp"
#include "game_globals.hpp"
#include "enemy.hpp"
#include "sm.hpp"
#include "entity.hpp"

using namespace std;
using namespace Foliage;

#define SHIP_SPEED Fixed(1.5f)
#define SHOT_SPEED Fixed(Sint16(10))
#define LASER_SPEED Fixed(Sint16(20))

LevelScene::LevelScene()
{
}

LevelScene::~LevelScene()
{
	delete font;
	delete map;
	delete ship;
	delete jaugeLaser;
}

void LevelScene::load()
{
	skipped = 0;
	hitCount = 0;

	hit = new Sound("break.wav");
	cout << "Hit SFX has " << hit->getSamplesNb() << " samples" << endl;

	font = new Font();
	map = new Map(BmpLoader::loadBmp("bg.bmp"));
	map->setScrollSpeed(2);

	ship = new AnimatedEntity(6);
	currentLevel->playerShip = ship;
	ship->addFrame(BmpLoader::loadBmp("vaiss00.bmp"));
	ship->addFrame(BmpLoader::loadBmp("vaiss01.bmp"));
	ship->addFrame(BmpLoader::loadBmp("vaiss02.bmp"));
	ship->addFrame(BmpLoader::loadBmp("vaiss03.bmp"));
	ship->addFrame(BmpLoader::loadBmp("vaiss04.bmp"));
	ship->addFrame(BmpLoader::loadBmp("vaiss05.bmp"));
	Rect shipHitbox = Rect(10, 11, 8, 9);
	ship->getHitbox()->addRect(shipHitbox);
	ship->setPosition(Point(100, 250));
	ship->setConstrained(true);
	fireShot0 = fireShot1 = fireLaser0 = fireLaser1 = false;
	framesBeforeLaser0 = framesBeforeLaser1 = -1;
	jaugeLaser = new Jauge(100);
	currentLevel->rythm = new RythmDisplay(); //TODO: non-consistent data placement
	//TODO: init currentLevel
	Enemy *e = new DonutEnemy();
	e->setPosition(Point(100, 40));
	currentLevel->enemies.push_back(e);
}

void LevelScene::start()
{
	cout << "Start music & game." << endl;
	currentLevel->rythm->startMusic();
	started = true;
}

void LevelScene::preRender()
{
	waitEndOfBg = map->asyncDraw();
}

void LevelScene::update()
{
	map->update();
	currentLevel->rythm->update();
	
	while (InputManager::numberOfEvents() > 0)
	{
		InputEvent e = InputManager::nextEvent();
		Fixed adj_speed = (e.getPushed()) ? SHIP_SPEED : SHIP_SPEED.opposite();
		if (e.getPlayer() == 1)
		{
			if (e.getButton() == LEFT)
			{
				s.x -= adj_speed;
			}
			else if (e.getButton() == RIGHT)
			{
				s.x += adj_speed;
			}
			else if (e.getButton() == UP)
			{
				s.y -= adj_speed;
			}
			else if (e.getButton() == DOWN)
			{
				s.y += adj_speed;
			}
			else if (e.getButton() == BUTTON1)
			{
				if (e.getPushed())
				{
					fireShot0 = true;
					fireLaser0 = false;
					framesBeforeLaser0 = 20;

					const Fixed closerEvent = currentLevel->rythm->toCloserEvent();
					Sint16 ce = Sint16(closerEvent);
					if (ce < 0)
					{
						ce = -ce;
					}
					jaugeLaser->setValue(ce);
					//cout << "closer event: " << closerEvent << endl;
				}
				else
				{
					fireShot0 = false;
					fireLaser0 = false;
					framesBeforeLaser0 = -1;
				}
			}
			else if (e.getButton() == BUTTON2)
			{
				if (e.getPushed())
				{
					fireShot1 = true;
					fireLaser1 = false;
					framesBeforeLaser1 = 20;
				}
				else
				{
					fireShot1 = false;
					fireLaser1 = false;
					framesBeforeLaser1 = -1;
				}
			}
		}
		else // player 2
		{
			if (e.getButton() == BUTTON3 && e.getPushed())
			{
				currentGame->show_hitbox = !currentGame->show_hitbox;
			}
			else if (e.getButton() == BUTTON1 && e.getPushed())
			{
			}
			else if (e.getButton() == BUTTON2 && e.getPushed())
			{
			}
		}
	}
	if (framesBeforeLaser0 >= 0)
	{
		if (framesBeforeLaser0 == 0)
		{
			fireLaser0 = true;
			fireLaser1 = false;
		}
		framesBeforeLaser0--;
	}
	if (framesBeforeLaser1 >= 0)
	{
		if (framesBeforeLaser1 == 0)
		{
			fireLaser0 = false;
			fireLaser1 = true;
		}
		framesBeforeLaser1--;
	}
	ship->setSpeed(s);
	ship->update();
	ListEnemy::iterator enn = currentLevel->enemies.begin();
	while (enn != currentLevel->enemies.end())
	{
		Enemy *e = *enn;
		e->update();
		if (e->hasDisappeared())
		{
			delete e;
			enn = currentLevel->enemies.erase(enn);
		}
		else
		{
			++enn;
		}
	}
	if (fireShot0)
	{
		if (!fireLaser0 && !fireLaser1)
		{
			Bullet *b1 = new Bullet(ship->getCenter(), F_3_PI_2, SHOT_SPEED, Bullet_Red);
			currentLevel->myBullets.push_back(b1);
		}
		Bullet *b2 = new Bullet(ship->getCenter(), F_3_PI_2 + F_0_DOT_1, SHOT_SPEED, Bullet_Red);
		Bullet *b3 = new Bullet(ship->getCenter(), F_3_PI_2 - F_0_DOT_1, SHOT_SPEED, Bullet_Red);
		currentLevel->myBullets.push_back(b2);
		currentLevel->myBullets.push_back(b3);
		fireShot0 = false;
	}	
	else if (fireShot1)
	{
		if (!fireLaser0 && !fireLaser1)
		{
			Bullet *b1 = new Bullet(ship->getCenter(), F_3_PI_2, SHOT_SPEED, Bullet_Green);
			currentLevel->myBullets.push_back(b1);
		}
		Bullet *b2 = new Bullet(ship->getCenter(), F_3_PI_2 + F_0_DOT_1, SHOT_SPEED, Bullet_Green);
		Bullet *b3 = new Bullet(ship->getCenter(), F_3_PI_2 - F_0_DOT_1, SHOT_SPEED, Bullet_Green);
		currentLevel->myBullets.push_back(b2);
		currentLevel->myBullets.push_back(b3);
		fireShot1 = false;
	}	
	if (fireLaser0)
	{
		Rect laser;
		laser.x = ship->getCenter().x - 5;
		laser.y = 0;
		laser.w = 11;
		laser.h = ship->getCenter().y - 15;
		Screen::fillRect(laser, Colors::Red);
	}
	else if (fireLaser1)
	{
		Rect laser;
		laser.x = ship->getCenter().x - 5;
		laser.y = 0;
		laser.w = 11;
		laser.h = ship->getCenter().y - 15;
		Screen::fillRect(laser, Colors::Green);
	}
	if ((currentLevel->rythm->hold(2) && (currentGame->frame % 5) == 0) || currentLevel->rythm->tap(2)) // pop new enemies
	{
		Enemy *e = new FlyerEnemy();
		//Sint16 sx = (rand() % 3) - 1;
		//Sint16 sy = (rand() % 4) + 1;
		//e->setPosition(Point((rand() % 200) + 20, 1 - e->getSize().h));
		//e->setSpeed(Speed(Fixed(sx), Fixed(sy)));
		e->setPosition(Point(220, -10));
		currentLevel->enemies.push_back(e);
	}
	frame_nb = "frame #";
	append_string(frame_nb, currentGame->frame);
	bullet_nb = "";
	append_string(bullet_nb, Sint32(currentLevel->enemyBullets.size()), 4);
	bullet_nb += " bullets";
	frameskip_nb = "frameskip ";
	append_string(frameskip_nb, skipped);
	waitEndOfBg();
}

void LevelScene::postRender()
{
	currentLevel->rythm->surface()->drawAt(Point(10, 150));
	ship->draw();
	if (currentGame->show_hitbox)
	{
		ship->drawHitbox(Colors::Green);
	}
	for (ListEnemy::const_iterator ennn = currentLevel->enemies.begin(); ennn != currentLevel->enemies.end(); ++ennn)
	{
		(*ennn)->display();
		if (currentGame->show_hitbox)
		{
			(*ennn)->drawHitbox(Colors::Black);
		}
	}
	ListBullet::iterator i = currentLevel->enemyBullets.begin();
	while (i != currentLevel->enemyBullets.end())
	{
		Bullet *b = *i;
		b->update();
		if (b->isDead())
		{
			delete b;
			i = currentLevel->enemyBullets.erase(i);
		}
		else
		{
			b->getEntity()->draw();
			if (currentGame->show_hitbox)
			{
				b->getEntity()->drawHitbox(Colors::Black);
			}
			if (Entity::collisionTest(ship, b->getEntity()))
			{
				delete b;
				i = currentLevel->enemyBullets.erase(i);
				hitCount++;
			}
			else
			{
				i++;
			}
		}
	}
	ListBullet::iterator j = currentLevel->myBullets.begin();
	while (j != currentLevel->myBullets.end())
	{
		Bullet *b = *j;
		b->update();
		if (b->getEntity()->outOfScreen())
		{
			j = currentLevel->myBullets.erase(j);
			delete b;
		}
		else
		{
			bool deleted = false;
			b->getEntity()->draw();
			for (ListEnemy::iterator enn = currentLevel->enemies.begin(); enn != currentLevel->enemies.end(); ++enn)
			{
				Enemy *enemy = *enn;
				if (enemy->collisionTest(b))
				{
					delete b;
					j = currentLevel->myBullets.erase(j);
					deleted = true;
					break;
				}
			}
			if (!deleted)
			{
				++j;
			}
		}
	}
	//Point lg(30, 40);
	//Point pc = currentLevel->playerShip->getCenter();
	//Laser l(lg, Point::angleBetween(lg, pc));
	//l.draw();
	//Screen::drawLine(lg, pc, Colors::Yellow);
	jaugeLaser->getUpdatedSurface()->drawAt(Point(10, 290));
	font->drawString(frame_nb, Point(1, 1));
	font->drawString(bullet_nb, Point(1, 10));
	font->drawString(frameskip_nb, Point(1, 19));
	/*
	if ((frame % 1000) == 0)
	{
		cout << enemyBullets.size() << " bullets!" << endl;
		printMemoryUsage();
	}
	*/
	if (currentGame->frame % 300 == 0)
	{
		//SoundManager::playSfx(&hit);
	}
	skipped = Screen::waitUntilEndOfFrame();
	Screen::flip();
	currentGame->frame++;
}
