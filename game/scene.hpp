#ifndef __SCENE
#define __SCENE

#include "foliage.hpp"
#include "font.hpp"
#include "map.hpp"
#include "jauge.hpp"
#include "rythm_display.hpp"

class Scene
{
public:
	virtual void load() = 0;
	virtual void update() = 0;
	virtual void preRender() = 0;
	virtual void postRender() = 0;

	bool isStarted() const
	{
		return started;
	}

protected:
	Scene()
	{
		started = false;
	}

	virtual ~Scene()
	{
	}

	bool started;
};

class LevelScene : public Scene
{
private:
	Sint32                   skipped;
	Font*                    font;
	Map*                     map;
	Foliage::AnimatedSprite* ship;
	Sint32                   hitCount;
	bool                     fireShot0, fireShot1, fireLaser0, fireLaser1;
	Sint32                   framesBeforeLaser0, framesBeforeLaser1;
	Jauge*                   jaugeLaser;
	Foliage::Speed           s;
	Foliage::Synchronizator  waitEndOfBg;
	Foliage::Sound*			 hit;
	std::string              frame_nb, bullet_nb, frameskip_nb;

public:
	LevelScene();
	~LevelScene();
	virtual void load();
	virtual void start();
	virtual void preRender();
	virtual void update();
	virtual void postRender();
};

#endif //__SCENE
