#ifndef __GAME_GLOBALS
#define __GAME_GLOBALS

#include <string>
#include <list>
#include "enemy.hpp"
#include "foliage.hpp"
#include "bullet.hpp"
#include "laser.hpp"

class Map;

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
	Game            *game;
	ListBullet       enemyBullets;
	ListBullet       myBullets;
	ListLaser        enemyLasers;
	Foliage::Sprite *playerShip;
	Sint32           time;
	ListEnemy        enemies;
	Map             *map;

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

extern Game *currentGame;
extern Level *currentLevel;

#endif //__GAME_GLOBALS
