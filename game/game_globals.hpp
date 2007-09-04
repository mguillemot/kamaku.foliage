#ifndef __GAME_GLOBALS
#define __GAME_GLOBALS

#include <list>
#include "foliage.hpp"

class Map;
class RythmDisplay;
class Bullet;
class Enemy;
class Laser;
class Entity;

struct Game
{
	Sint32 lives;
	Sint32 score;
	bool   show_hitbox;
	Sint32 frame;

	void reset()
	{
		lives = 3;
		score = 0;
		show_hitbox = false;
	}
};

struct Level
{
public:
	Game*            game;
	std::list<Bullet*> enemyBullets;
	std::list<Bullet*> myBullets;
	std::list<Laser*> enemyLasers;
	Entity*          playerShip;
	Sint32           time;
	std::list<Enemy*> enemies;
	Map*             map;
	RythmDisplay*    rythm;
	Foliage::Point   viewPort;

	void reset()
	{
		/*for (ListBullet::iterator i = enemyBullets.begin(); i != enemyBullets.end(); ++i)
		{
			delete *i;
		}*/
		enemyBullets.clear();
		/*for (ListBullet::iterator i = myBullets.begin(); i != myBullets.end(); ++i)
		{
			delete *i;
		}*/
		myBullets.clear();
		// todo
		enemyLasers.clear();
		time = 0;
		enemies.clear();
	}
};

extern Game* currentGame;
extern Level* currentLevel;

#endif //__GAME_GLOBALS
