#ifndef __MAP
#define __MAP

#include "foliage.hpp"
#include "game_globals.hpp"

class Map
{
public:
	Map(Foliage::Surface *map)
	{
		_map = map;
		_scrollSpeed = 1;
		#ifdef __PPC
			_map1 = _map->createNewShiftedSurface(1);
			_map2 = _map->createNewShiftedSurface(2);
			_map3 = _map->createNewShiftedSurface(3);
		#endif
		_scroll = _map->getSize().h - 320;
		_currentWidth = _map->getSize().w;
	}

	Sint16 currentWidth() const
	{
		return _currentWidth;
	}

	Sint16 maxShift() const
	{
		return _currentWidth - Foliage::Screen::Width;
	}

	Sint16 currentShift() const
	{	
		return _currentShift;
	}

	Foliage::Synchronizator asyncDraw() const
	{
		Foliage::Rect section;
		section.w = 240;
		section.h = 320;
		section.x = currentShift();
		section.y = _scroll;
		if (section.y < 0)
		{
			section.y = 0;
		}
		
		#ifdef __PPC__
			if (section.y % 4 == 0)
			{
				return Foliage::Screen::asyncBlitSection(_map, section, Foliage::Point(0, 0));
			}
			else if (section.y % 4 == 1)
			{
				return Foliage::Screen::asyncBlitSection(_map3, section, Foliage::Point(0, 0));
			}
			else if (section.y % 4 == 2)
			{
				return Foliage::Screen::asyncBlitSection(_map2, section, Foliage::Point(0, 0));
			}
			else
			{
				return Foliage::Screen::asyncBlitSection(_map1, section, Foliage::Point(0, 0));
			}
		#else
			return Foliage::Screen::asyncBlitSection(_map, section, Foliage::Point(0, 0));
		#endif
	}

	void setScrollSpeed(const Sint32 speed)
	{
		_scrollSpeed = speed;
		_scrollSpeedDecr = speed;
	}

	void update()
	{
		_scrollSpeedDecr--;
		if (_scrollSpeedDecr <= 0)
		{
			_scrollSpeedDecr = _scrollSpeed;
			if (_scroll > 0)
			{
				_scroll--;
			}
		}
		const Foliage::Point pos = currentLevel->playerShip->getPosition();
		const Foliage::Rect shipHitbox = currentLevel->playerShip->getHitbox()->rects()[0];
		Foliage::Fixed percent(pos.x);
		percent += shipHitbox.x;
		percent /= Foliage::Fixed(Sint16(Foliage::Screen::Width - shipHitbox.w));
		percent *= maxShift();
		_currentShift = Sint16(percent);
	}

protected:
	Foliage::Surface *_map;
	#ifdef __PPC__
		Foliage::Surface *_map1;
		Foliage::Surface *_map2;
		Foliage::Surface *_map3;
	#endif
	Sint16            _scroll;
	Sint32            _scrollSpeed, _scrollSpeedDecr;
	Sint16            _currentWidth;
	Sint16            _currentShift;
};

#endif //__MAP
