#ifndef __JAUGE
#define __JAUGE

#include "foliage.hpp"

#define JAUGE_WIDTH  100
#define JAUGE_HEIGHT 20

class Jauge
{
public:
	Jauge(const Sint32 max)
		: _max(max), _value(0)
	{
		_representation = Foliage::Surface::createSurface(Foliage::Size(JAUGE_WIDTH, JAUGE_HEIGHT), "jauge");
	}

	void setValue(const Sint32 value) { _value = value; }
	Sint32 getValue() const { return _value; }

	Foliage::Surface *getUpdatedSurface
	{
		_representation->fill(Foliage::Colors::Transparent);

	}

private:
	Sint32 _value;
	Sint32 _max;
	Foliage::Surface *_representation;
};

#endif //__JAUGE
