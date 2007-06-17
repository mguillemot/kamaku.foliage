#include <string>
#include <iostream>
#include <list>
#include "foliage.hpp"
#include "bullet.hpp"
#include "font.hpp"
#include "gamecolor.hpp"
#include "enemy.hpp"
#include "rythm_display.hpp"
#include "game_globals.hpp"
#include "laser.hpp"
#include "map.hpp"
#include "jauge.hpp"
#include "sm.hpp"

#ifdef main
#undef main
#endif

using namespace std;
using namespace Foliage;

void append_string(string &s, Sint32 n, Sint32 min_len = 1)
{
	char buf[10]; // max=2147483647
	int buf_len = 0;
	if (n == 0)
	{
		s += '0';
		return;
	}
	else if (n < 0)
	{
		s += '-';
		n = -n;
	}
	while (n != 0)
	{
		int digit = n % 10;
		n /= 10;
		buf[buf_len++] = '0' + digit;
	}
	for (Sint32 i = 0; i < (min_len - buf_len - 1); ++i)
	{
		s += '0';
	}
	while (buf_len > 0)
	{
		s += buf[--buf_len];
	}
}

void start()
{
	Foliage::Sound hit("break.wav");
	cout << "SFX has " << hit.getSamplesNb() << " samples" << endl;
	//SoundManager::disableSound();

	Bullet::loadBulletSurfaces();
	ColoredEnemy::loadSurfaces();
	DonutEnemy::loadSurfaces();
	FlyerEnemy::loadSurfaces();
	TankEnemy::loadSurfaces();

	currentGame = new Game();
	currentGame->reset();
	currentLevel = new Level();
	currentLevel->reset();

	Sint32 skipped = 0;
    Font font;
    cout << "Font loaded." << endl;
	Map map(BitmapLoader::loadBitmap("bg.bmp"));

	Foliage::AnimatedSprite ship(6);
	currentLevel->playerShip = &ship;
	ship.addFrame(BitmapLoader::loadBitmap("vaiss00.bmp"));
	ship.addFrame(BitmapLoader::loadBitmap("vaiss01.bmp"));
	ship.addFrame(BitmapLoader::loadBitmap("vaiss02.bmp"));
	ship.addFrame(BitmapLoader::loadBitmap("vaiss03.bmp"));
	ship.addFrame(BitmapLoader::loadBitmap("vaiss04.bmp"));
	ship.addFrame(BitmapLoader::loadBitmap("vaiss05.bmp"));
    Rect shipHitbox = Rect(10, 11, 8, 9);
	ship.getHitbox()->addRect(shipHitbox);
    ship.setPosition(Point(100, 250));
	ship.setConstrained(true);
	Speed s;
	const Fixed ShipSpeed = Fixed(1.5f);
	Sint32 hitCount = 0;
	Sint32 randlevel = 10;
	bool fireShot0 = false, fireShot1 = false, fireLaser0 = false, fireLaser1 = false;
	Sint32 framesBeforeLaser0 = -1, framesBeforeLaser1 = -1;
	const Fixed ShotSpeed = Fixed(Sint16(10));
	const Fixed LaserSpeed = Fixed(Sint16(20));
	Jauge jaugeLaser(100);
	currentLevel->rythm = new RythmDisplay();
	Enemy *e = new DonutEnemy();
	e->setPosition(Point(100, 40));
	currentLevel->enemies.push_back(e);

	cout << "Start music." << endl;
	currentLevel->rythm->startMusic();
	
	cout << "Start game!" << endl;
	while (true)
	{
		Synchronizator waitEndOfBg = map.asyncDraw();

		map.update();
		currentLevel->rythm->update();
		
		while (Foliage::InputManager::numberOfEvents() > 0)
		{
			InputEvent e = Foliage::InputManager::nextEvent();
			Fixed adj_speed = (e.getPushed()) ? ShipSpeed : ShipSpeed.opposite();
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

						const Foliage::Fixed closerEvent = currentLevel->rythm->toCloserEvent();
						Sint16 ce = Sint16(closerEvent);
						if (ce < 0)
						{
							ce = -ce;
						}
						jaugeLaser.setValue(ce);
						cout << "closer event: " << closerEvent << endl;
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
					randlevel++;
				}
				else if (e.getButton() == BUTTON2 && e.getPushed())
				{
					if (randlevel >= 2)
					{
						randlevel--;
					}
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
		ship.setSpeed(s);
		ship.update();
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
				Bullet *b1 = new Bullet(ship.getCenter(), F_3_PI_2, ShotSpeed, Bullet_Red);
				currentLevel->myBullets.push_back(b1);
			}
			Bullet *b2 = new Bullet(ship.getCenter(), F_3_PI_2 + F_0_DOT_1, ShotSpeed, Bullet_Red);
			Bullet *b3 = new Bullet(ship.getCenter(), F_3_PI_2 - F_0_DOT_1, ShotSpeed, Bullet_Red);
			currentLevel->myBullets.push_back(b2);
			currentLevel->myBullets.push_back(b3);
			fireShot0 = false;
		}	
		else if (fireShot1)
		{
			if (!fireLaser0 && !fireLaser1)
			{
				Bullet *b1 = new Bullet(ship.getCenter(), F_3_PI_2, ShotSpeed, Bullet_Green);
				currentLevel->myBullets.push_back(b1);
			}
			Bullet *b2 = new Bullet(ship.getCenter(), F_3_PI_2 + F_0_DOT_1, ShotSpeed, Bullet_Green);
			Bullet *b3 = new Bullet(ship.getCenter(), F_3_PI_2 - F_0_DOT_1, ShotSpeed, Bullet_Green);
			currentLevel->myBullets.push_back(b2);
			currentLevel->myBullets.push_back(b3);
			fireShot1 = false;
		}	
		if (fireLaser0)
		{
			Rect laser;
			laser.x = ship.getCenter().x - 5;
			laser.y = 0;
			laser.w = 11;
			laser.h = ship.getCenter().y - 15;
			Screen::fillRect(laser, Colors::Red);
		}
		else if (fireLaser1)
		{
			Rect laser;
			laser.x = ship.getCenter().x - 5;
			laser.y = 0;
			laser.w = 11;
			laser.h = ship.getCenter().y - 15;
			Screen::fillRect(laser, Colors::Green);
		}
		if ((currentGame->frame % 25) == 0) // pop new enemies//TEMP:desactivated
		{
			GameColor c;
			Sint32 r = rand() % 100;
			if (r < 20)
			{
				c = Red;
			}
			else if (r < 40)
			{
				c = Green;
			}
			else if (r < 60)
			{
				c = Blue;
			}
			else if (r < 70)
			{
				c = Yellow;
			}
			else if (r < 80)
			{
				c = Purple;
			}
			else if (r < 90)
			{
				c = Lightblue;
			}
			else if (r < 95)
			{
				c = White;
			}
			else
			{
				c = Black;
			}
			//Enemy *e = new ColoredEnemy(c);
			Enemy *e = new FlyerEnemy();
			Sint16 sx = (rand() % 3) - 1;
			Sint16 sy = (rand() % 4) + 1;
			e->setPosition(Point((rand() % 200) + 20, 1 - e->getSize().h));
			e->setSpeed(Speed(Fixed(sx), Fixed(sy)));
			currentLevel->enemies.push_back(e);
		}
		string frame_nb = "frame #";
		append_string(frame_nb, currentGame->frame);
		string bullets_nb;
		append_string(bullets_nb, Sint32(currentLevel->enemyBullets.size()), 4);
		bullets_nb += " bullets";
		string frameskip_nb = "frameskip ";
		append_string(frameskip_nb, skipped);
		string randlevel_nb = "level ";
		append_string(randlevel_nb, randlevel);
		waitEndOfBg();
		
		// BEGIN DRAWING SPRITES NOW!
		
		currentLevel->rythm->surface()->drawAt(Point(10, 150));
		ship.draw();
		if (currentGame->show_hitbox)
		{
			ship.drawHitbox(Colors::Green);
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
				b->getSprite()->draw();
				if (currentGame->show_hitbox)
				{
					b->getSprite()->drawHitbox(Colors::Black);
				}
				if (Sprite::collisionTest(&ship, b->getSprite()))
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
			if (b->getSprite()->outOfScreen())
			{
				j = currentLevel->myBullets.erase(j);
				delete b;
			}
			else
			{
				bool deleted = false;
				b->getSprite()->draw();
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
		Point lg(30, 40);
		Point pc = currentLevel->playerShip->getCenter();
		Laser l(lg, Point::angleBetween(lg, pc));
		l.draw();
		Screen::drawLine(lg, pc, Colors::Yellow);
		jaugeLaser.getUpdatedSurface()->drawAt(Point(10, 290));
		font.drawString(frame_nb, Point(1, 1));
		font.drawString(bullets_nb, Point(1, 10));
		font.drawString(frameskip_nb, Point(1, 19));
		font.drawString(randlevel_nb, Point(1, 28));
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
		if (skipped > 0)
		{
			Screen::setPixel(Point(0, 0), Colors::Green);
			//std::cout << "skipped " << skipped << std::endl;
		}
		else
		{
			Screen::setPixel(Point(0, 0), Colors::Black);
		}
		Screen::flip();
		currentGame->frame++;
	}
}

int main()
{
	Foliage::init();
	start();
	while (true)
	{
	}
	return 0;
}
