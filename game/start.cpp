#include "foliage.hpp"
#include "scene.hpp"

#ifdef main
#undef main
#endif

int main()
{
	// System init
	Foliage::init();

	// Game loading & init
	Bullet::loadBulletSurfaces();
	DonutEnemy::loadSurfaces();
	FlyerEnemy::loadSurfaces();
	TankEnemy::loadSurfaces();
	currentGame = new Game();
	currentGame->reset();
	currentLevel = new Level();
	currentLevel->reset();

	// Create & play scenes
	LevelScene demoLevel;
	demoLevel.load();
	demoLevel.start();
	while (true)
	{
		demoLevel.preRender();
		demoLevel.update();
		demoLevel.postRender();
	}

	// Unreachable end-of-game loop
	while (true)
	{
	}
	return 0;

}

