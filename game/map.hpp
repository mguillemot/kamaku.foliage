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
			return Foliage::Screen::asyncBlitSection(_map, section, Foliage::Point(0, 0));
		#endif
	}

	void update()
	{
		if (_scroll > 0)
		{
			_scroll--;
		}
		const Foliage::Point pos = currentLevel->playerShip->getPosition();
		const Foliage::Size size = currentLevel->playerShip->getSize();
		Foliage::Fixed percent(pos.x);
		percent /= Foliage::Fixed(Sint16(Foliage::Screen::Width - size.w));
		percent *= maxShift();
		_currentShift = Sint16(percent);
	}

protected:
	Foliage::Surface *_map;
	Sint16            _scroll;
	Sint16            _currentWidth;
	Sint16            _currentShift;
};

#endif //__MAP
