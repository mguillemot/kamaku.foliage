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
	/*
	Foliage::Sound bg("kloops.wav");
	cout << "bg music has " << bg.getSamplesNb() << " samples" << endl;
	Foliage::Sound hit("break.wav");
	cout << "SFX has " << hit.getSamplesNb() << " samples" << endl;
	*/
	SoundManager::disableSound();

	Bullet::loadBulletSurfaces();
	Enemy::loadSurfaces();

	currentGame = new Game();
	currentGame->reset();
	currentLevel = new Level();
	currentLevel->reset();

	Sint32 skipped = 0;
    Font font;
    cout << "Font loaded." << endl;
    Foliage::Sprite background("bg.bmp"); // 300x3910
	#ifdef __PPC
		Foliage::Sprite background1(background.getCurrentSurface()->createNewShiftedSurface(1));
		Foliage::Sprite background2(background.getCurrentSurface()->createNewShiftedSurface(2));
		Foliage::Sprite background3(background.getCurrentSurface()->createNewShiftedSurface(3));
	#endif
	Foliage::Sprite ship("vaiss00.bmp");
	currentLevel->playerShip = &ship;
	ship.addFrame("vaiss01.bmp");
	ship.addFrame("vaiss02.bmp");
	ship.addFrame("vaiss03.bmp");
	ship.addFrame("vaiss04.bmp");
	ship.addFrame("vaiss05.bmp");
    Rect shipHitbox = Rect(10, 11, 8, 9);
    ship.setHitbox(shipHitbox);
    ship.setPosition(Point(100, 250));
	Speed s;
	const Fixed ShipSpeed = Fixed(1.5f);
	Sint32 hitCount = 0;
	Sint32 randlevel = 10;
	bool fireShot0 = false, fireShot1 = false, fireLaser0 = false, fireLaser1 = false;
	Sint32 framesBeforeLaser0 = -1, framesBeforeLaser1 = -1;
	const Fixed ShotSpeed = Fixed(Sint16(10));
	const Fixed LaserSpeed = Fixed(Sint16(20));
	RythmDisplay rythm;

	Enemy *e = new Enemy();
	e->setPosition(Point(100, 100));
	currentLevel->enemies.push_back(e);

	//SoundManager::playBg(&bg);		
	
	while (true)
	{
		rythm.nextFrame();

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
		Rect section;
		section.w = 240;
		section.h = 320;
		section.x = ship.getPosition().x / 4;
		section.y = background.getSize().h - 320 - currentGame->frame;
		if (section.y < 0)
			section.y = 0;
		Synchronizator waitEndOfBg;
		#ifdef __PPC__
			if (section.y % 4 == 0)
			{
				waitEndOfBg = Screen::asyncBlitSection(background.getCurrentSurface(), section, Point(0, 0));
			}
			else if (section.y % 4 == 1)
			{
				waitEndOfBg = Screen::asyncBlitSection(background3.getCurrentSurface(), section, Point(0, 0));
			}
			else if (section.y % 4 == 2)
			{
				waitEndOfBg = Screen::asyncBlitSection(background2.getCurrentSurface(), section, Point(0, 0));
			}
			else
			{
				waitEndOfBg = Screen::asyncBlitSection(background1.getCurrentSurface(), section, Point(0, 0));
			}
		#else
			waitEndOfBg = Screen::asyncBlitSection(background.getCurrentSurface(), section, Point(0, 0));
		#endif
		ship.move();
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
		if ((currentGame->frame % 25) == 250) // pop new enemies//TEMP:desactivated
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
			Enemy *e = new Enemy(c);
			Sint16 sx = (rand() % 3) - 1;
			Sint16 sy = (rand() % 4) + 1;
			e->setPosition(Point((rand() % 150) + 1 - e->getSize().h, 0));
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
		
		rythm.getUpdatedSurface()->drawAt(Point(20, 200));
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
		const Rect shipHb = ship.getScreenHitbox();
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
				Rect bHb = b->getSprite()->getScreenHitbox();
				if (Rect::intersects(shipHb, bHb))
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
				const Rect bHb = b->getSprite()->getScreenHitbox();
				for (ListEnemy::iterator enn = currentLevel->enemies.begin(); enn != currentLevel->enemies.end(); ++enn)
				{
					const bool hit = (*enn)->collisionTest(b);
					if (hit)
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
		font.drawString(frame_nb, Point(0, 0));
		font.drawString(bullets_nb, Point(0, 17));
		font.drawString(frameskip_nb, Point(0, 34));
		font.drawString(randlevel_nb, Point(0, 51));
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
