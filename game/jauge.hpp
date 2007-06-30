#ifndef __JAUGE
#define __JAUGE

#include "foliage.hpp"

#define JAUGE_WIDTH  100
#define JAUGE_HEIGHT 20

class Jauge
{
public:
	Jauge(const Sint32 max)
	{
		_max = max;
		_value = 0;
		_representation = new Foliage::Surface(Foliage::Size(JAUGE_WIDTH, JAUGE_HEIGHT), "jauge");
	}

	void setValue(const Sint32 value)
	{ 
		if (value > _max)
		{
			_value = _max;
		}
		else if (value < 0)
		{
			_value = 0;
		}
		else
		{
			_value = value;
		}
	}
	
	Sint32 getValue() const 
	{ 
		return _value; 
	}

	Foliage::Surface *getUpdatedSurface()
	{
		_representation->fill(Foliage::Colors::Transparent);
		_representation->drawRect(Foliage::Rect(0, 0, JAUGE_WIDTH, JAUGE_HEIGHT), Foliage::Colors::Black);
		Foliage::Fixed percent(_value);
		percent /= _max;
		percent *= Sint16(JAUGE_WIDTH);
		Sint16 l = Sint16(percent) - 1;
		if (l == JAUGE_WIDTH - 1)
		{
			l--;
		}
		_representation->fillRect(Foliage::Rect(1, 1, l, JAUGE_HEIGHT - 2), Foliage::Colors::Red);
		return _representation;
	}

private:
	Sint16 _value;
	Sint16 _max;
	Foliage::Surface *_representation;
};

#endif //__JAUGE
