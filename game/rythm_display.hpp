#ifndef __RYTHM_DISPLAY
#define __RYTHM_DISPLAY

#include <deque>
#include "foliage.hpp"

class RythmDisplay
{
public:
	RythmDisplay()
	{
		_representation = Foliage::Surface::createSurface(Foliage::Size(50, 100), "rythm");
		Foliage::Fixed beat;
		const Foliage::Fixed interval = Foliage::Fixed(Sint16(60));
		for (int i = 0; i < 100; i++)
		{
			_beats.push_front(beat);
			beat += interval;
		}
	}

	~RythmDisplay()
	{
		// delete surface?
	}

	Foliage::Surface *getUpdatedSurface()
	{
		_representation->fill(Foliage::Colors::Transparent);
		for (int i = 0; i < 10; i++)
		{
			_representation->setPixel(Foliage::Point(i, i), Foliage::Colors::Blue);
		}
		return _representation;
	}

private:
	Foliage::Fixed             _current;
	std::deque<Foliage::Fixed> _beats;
	Foliage::Surface          *_representation;
};

#endif //__RYTHM_DISPLAY
