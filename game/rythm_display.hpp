#ifndef __RYTHM_DISPLAY
#define __RYTHM_DISPLAY

#include <deque>
#include "foliage.hpp"

#define RYTHMDISPLAY_WIDTH  50
#define RYTHMDISPLAY_HEIGHT 100

typedef std::deque<Foliage::Fixed> Beats;

class RythmDisplay
{
public:
	RythmDisplay()
	{
		_representation = Foliage::Surface::createSurface(Foliage::Size(RYTHMDISPLAY_WIDTH, RYTHMDISPLAY_HEIGHT), "rythm");
		Foliage::Fixed beat;
		const Foliage::Fixed interval = Foliage::Fixed(Sint16(60)); //TEMP
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
		_representation->drawRect(Foliage::Rect(0, 0, RYTHMDISPLAY_WIDTH, RYTHMDISPLAY_HEIGHT), Foliage::Colors::Blue);
		_representation->drawLine(Foliage::Point(0, 0), Foliage::Point(RYTHMDISPLAY_WIDTH - 1, RYTHMDISPLAY_HEIGHT - 1), Foliage::Colors::Blue);
		_representation->drawLine(Foliage::Point(0, RYTHMDISPLAY_HEIGHT - 1), Foliage::Point(RYTHMDISPLAY_WIDTH - 1, 0), Foliage::Colors::Blue);
		_representation->drawLine(Foliage::Point(RYTHMDISPLAY_WIDTH / 2, 0), Foliage::Point(RYTHMDISPLAY_WIDTH / 2, RYTHMDISPLAY_HEIGHT - 1), Foliage::Colors::White);
		for (Beats::const_iterator i = _beats.begin(); i != _beats.end(); ++i)
		{
			Sint16 delay = Sint16(*i - _current);
			if (delay >= 0 && delay < RYTHMDISPLAY_HEIGHT)
			{
				delay = RYTHMDISPLAY_HEIGHT - delay - 1;
				_representation->drawLine(Foliage::Point(20, delay), Foliage::Point(30, delay), Foliage::Colors::Black);
			}
		}
		return _representation;
	}

	void nextFrame()
	{
		_current += 1;
	}

private:
	Foliage::Fixed    _current;
	Beats             _beats;
	Foliage::Surface *_representation;
};

#endif //__RYTHM_DISPLAY
