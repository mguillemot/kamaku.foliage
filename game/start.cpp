#include <string>
#include <iostream>
#include <list>
#include "foliage.hpp"
#include "bullet.hpp"
#include "font.hpp"
#include "gamecolor.hpp"
#include "enemy.hpp"

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

ListBullet enemyBullets, myBullets;
Sprite *playerShip = NULL;
bool show_hitbox = false;

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

	Sint32 skipped = 0;
    Font font;
    cout << "Font loaded." << endl;
    Foliage::Sprite background("bg.bmp"); // 300x3910
	#ifdef __PPC
		Foliage::Sprite background1(background.getCurrentSurface()->createNewShiftedSurface(1));
		Foliage::Sprite background2(background.getCurrentSurface()->createNewShiftedSurface(2));
		Foliage::Sprite background3(background.getCurrentSurface()->createNewShiftedSurface(3));
	#endif
	ListEnemy enemies;
	Foliage::Sprite ship("vaiss00.bmp");
	playerShip = &ship;
	ship.addFrame("vaiss01.bmp");
	ship.addFrame("vaiss02.bmp");
	ship.addFrame("vaiss03.bmp");
	ship.addFrame("vaiss04.bmp");
	ship.addFrame("vaiss05.bmp");
    Rect shipHitbox = Rect(10, 11, 8, 9);
    ship.setHitbox(shipHitbox);
    ship.setPosition(Point(100, 250));
	Speed s;
	Sint32 frame = 0;
	const Fixed ShipSpeed = Fixed(1.5f);
	Sint32 hitCount = 0;
	Sint32 randlevel = 10;
	bool fireShot0 = false, fireShot1 = false;

	//SoundManager::playBg(&bg);		
	
	while (true)
	{
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
					fireShot0 = e.getPushed();
				}
				else if (e.getButton() == BUTTON2)
				{
					fireShot1 = e.getPushed();
				}
			}
			else // player 2
			{
				if (e.getButton() == BUTTON3 && e.getPushed())
				{
					show_hitbox = !show_hitbox;
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
		ship.setSpeed(s);
		Rect section;
		section.w = 240;
		section.h = 320;
		section.x = ship.getPosition().x / 4;
		section.y = background.getSize().h - 320 - frame;
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
		ListEnemy::iterator enn = enemies.begin();
		while (enn != enemies.end())
		{
			Enemy *e = *enn;
			e->getSprite()->move();
			if (e->getKilled() || e->getSprite()->outOfScreen())
			{
				delete e;
				enn = enemies.erase(enn);
			}
			else
			{
				e->update();
				++enn;
			}
		}
		if (fireShot0)
		{
			Bullet *b1 = new Bullet(ship.getCenter(), F_3_PI_2, Fixed(Sint16(20)), 4);
			Bullet *b2 = new Bullet(ship.getCenter(), F_3_PI_2 + F_0_DOT_1, Fixed(Sint16(20)), 4);
			Bullet *b3 = new Bullet(ship.getCenter(), F_3_PI_2 - F_0_DOT_1, Fixed(Sint16(20)), 4);
			myBullets.push_back(b1);
			myBullets.push_back(b2);
			myBullets.push_back(b3);
		}	
		else if (fireShot1)
		{
			Bullet *b1 = new Bullet(ship.getCenter(), F_3_PI_2, Fixed(Sint16(20)), 5);
			Bullet *b2 = new Bullet(ship.getCenter(), F_3_PI_2 + F_0_DOT_1, Fixed(Sint16(20)), 5);
			Bullet *b3 = new Bullet(ship.getCenter(), F_3_PI_2 - F_0_DOT_1, Fixed(Sint16(20)), 5);
			myBullets.push_back(b1);
			myBullets.push_back(b2);
			myBullets.push_back(b3);
		}	
		if ((frame % 25) == 0) // pop new enemies
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
			e->getSprite()->setPosition(Point((rand() % 150) + 1 - e->getSprite()->getSize().h, 0));
			e->getSprite()->setSpeed(Speed(Fixed(sx), Fixed(sy)));
			enemies.push_back(e);
		}
		string frame_nb = "frame #";
		append_string(frame_nb, frame);
		string bullets_nb;
		append_string(bullets_nb, Sint32(enemyBullets.size()), 4);
		bullets_nb += " bullets";
		string frameskip_nb = "frameskip ";
		append_string(frameskip_nb, skipped);
		string randlevel_nb = "level ";
		append_string(randlevel_nb, randlevel);
		waitEndOfBg();
		
		// BEGIN DRAWING SPRITES NOW!
		
		ship.draw();
		if (show_hitbox)
		{
			ship.drawHitbox(Colors::Green);
		}
		for (ListEnemy::const_iterator ennn = enemies.begin(); ennn != enemies.end(); ++ennn)
		{
			(*ennn)->getSprite()->draw();
		}
		const Rect shipHb = ship.getScreenHitbox();
		ListBullet::iterator i = enemyBullets.begin();
		while (i != enemyBullets.end())
		{
			Bullet *b = *i;
			b->update();
			if (b->getSprite()->outOfScreen())
			{
				delete b;
				i = enemyBullets.erase(i);
			}
			else
			{
				b->getSprite()->draw();
				Rect bHb = b->getSprite()->getScreenHitbox();
				if (Rect::intersects(shipHb, bHb))
				{
					delete b;
					i = enemyBullets.erase(i);
					hitCount++;
				}
				else
				{
					i++;
				}
			}
		}
		ListBullet::iterator j = myBullets.begin();
		while (j != myBullets.end())
		{
			Bullet *b = *j;
			b->update();
			if (b->getSprite()->outOfScreen())
			{
				j = myBullets.erase(j);
				delete b;
			}
			else
			{
				bool deleted = false;
				b->getSprite()->draw();
				const Rect bHb = b->getSprite()->getScreenHitbox();
				for (ListEnemy::iterator enn = enemies.begin(); enn != enemies.end(); ++enn)
				{
					const Rect eHb = (*enn)->getSprite()->getScreenHitbox();
					if (Rect::intersects(bHb, eHb))
					{
						j = myBullets.erase(j);
						delete b;
						(*enn)->damage(25);
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
		if (frame % 300 == 0)
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
		frame++;
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
