#ifndef __RYTHM_DISPLAY
#define __RYTHM_DISPLAY

#include <iostream>
#include <deque>
#include "foliage.hpp"
#include "sm.hpp"

#define RYTHMDISPLAY_WIDTH  50
#define RYTHMDISPLAY_HEIGHT 100

enum Events
{
	EVENT_NONE = 0,
	EVENT_MAIN = 1,
	EVENT_SUB = 2
};

struct Beat
{
	Foliage::Fixed time;
	Uint32 events;
};

typedef std::deque<Beat> Beats;

class RythmDisplay
{
public:
	RythmDisplay()
		: _music("maison.wav")
	{
		//TEMP: (maison specific)
		std::cout << "bg music has " << _music.getSamplesNb() << " samples" << std::endl;
		_sm = new Sm::SMfile();
		Sm::load_sm(_sm, "maison.sm");
		_measure = 0;
		std::cout << _sm->level[0].measure_nb << " measures." << std::endl;
		_representation = Foliage::Surface::createEmptySurface(Foliage::Size(RYTHMDISPLAY_WIDTH, RYTHMDISPLAY_HEIGHT), "rythm");
		const Foliage::Fixed bpm = Foliage::Fixed(333.3333333f);
		const Foliage::Fixed gap = Foliage::Fixed(-21.0f);
		one_frame = Foliage::Fixed(16.77777777f);
		Beat b;
		b.time = gap;
		const Foliage::Fixed measure_length = bpm * Sint16(4);
		for (Sint32 i = 0; i < _sm->level[0].measure_nb; i++)
		{
			const Foliage::Fixed note_length = measure_length / Sint16(_sm->level[0].measure[i].notes_nb);
			for (Sint32 j = 0; j < _sm->level[0].measure[i].notes_nb; j++)
			{
				b.events = EVENT_NONE;
				if (_sm->level[0].measure[i].notes[j].tap[Sm::LEFT_A] == Sm::TAP)
				{
					b.events = EVENT_MAIN;
					_beats.push_back(b);
				}
				else if (_sm->level[0].measure[i].notes[j].tap[Sm::RIGHT_A] == Sm::TAP)
				{
					_beats.push_back(b);
				}
				else if (_sm->level[0].measure[i].notes[j].tap[Sm::UP_A] == Sm::TAP)
				{
					_beats.push_back(b);
				}
				else if (_sm->level[0].measure[i].notes[j].tap[Sm::DOWN_A] == Sm::TAP)
				{
					b.events = EVENT_SUB;
					_beats.push_back(b);
				}
				b.time += note_length;
			}
		}
		_temp = 4;
		_lastEvent = Sint16(-1000);
	}

	~RythmDisplay()
	{
		// delete surface?
	}

	void update()
	{
		// Update internal state
		_events = EVENT_NONE;
		_thisFrame = Foliage::Fixed(Sint16(_timer.duration() / 1000));
		Beats::iterator i = _beats.begin();
		while (i != _beats.end())
		{
			Foliage::Fixed fdelay = i->time - _thisFrame;
			Sint16 delay = Sint16(fdelay / one_frame);
			if (delay < 0)
			{
				_temp--;
				/*
				if (_temp == 0)
				{
					_temp = 4;
					_events |= EVENT_MAIN;
				}
				else if (_temp == 2)
				{
					_events |= EVENT_SUB;
				}
				*/
				_events = i->events;
				_lastEvent = i->time;
				i = _beats.erase(i);
			}
			else if (delay >= 0 && delay < RYTHMDISPLAY_HEIGHT)
			{
				delay = RYTHMDISPLAY_HEIGHT - delay - 1;
				_representation->drawLine(Foliage::Point(20, delay), Foliage::Point(30, delay), Foliage::Colors::Black);
				++i;
			}
			else
			{
				break;
			}
		}

		// Update representation
		_representation->fill(Foliage::Colors::Transparent);
		_representation->drawRect(Foliage::Rect(0, 0, RYTHMDISPLAY_WIDTH, RYTHMDISPLAY_HEIGHT), Foliage::Colors::Blue);
		_representation->drawLine(Foliage::Point(0, 0), Foliage::Point(RYTHMDISPLAY_WIDTH - 1, RYTHMDISPLAY_HEIGHT - 1), Foliage::Colors::Blue);
		_representation->drawLine(Foliage::Point(0, RYTHMDISPLAY_HEIGHT - 1), Foliage::Point(RYTHMDISPLAY_WIDTH - 1, 0), Foliage::Colors::Blue);
		_representation->drawLine(Foliage::Point(RYTHMDISPLAY_WIDTH / 2, 0), Foliage::Point(RYTHMDISPLAY_WIDTH / 2, RYTHMDISPLAY_HEIGHT - 1), Foliage::Colors::White);
		i = _beats.begin();
		while (i != _beats.end())
		{
			Foliage::Fixed fdelay = i->time - _thisFrame;
			Sint16 delay = Sint16(fdelay / one_frame);
			if (delay < 0)
			{
				//TEMP
				std::cout << "ERROR: delay should not be <0 while updating representation." << std::endl;
				break;
			}
			else if (delay < RYTHMDISPLAY_HEIGHT)
			{
				delay = RYTHMDISPLAY_HEIGHT - delay - 1;
				_representation->drawLine(Foliage::Point(20, delay), Foliage::Point(30, delay), Foliage::Colors::Black);
				++i;
			}
			else
			{
				break;
			}
		}
		if (_events & EVENT_MAIN)
		{
			_representation->fillRect(Foliage::Rect(0, 0, RYTHMDISPLAY_WIDTH, RYTHMDISPLAY_HEIGHT), Foliage::Colors::Red);
		}
		else if (_events & EVENT_SUB)
		{
			_representation->fillRect(Foliage::Rect(0, 0, RYTHMDISPLAY_WIDTH, RYTHMDISPLAY_HEIGHT), Foliage::Colors::Blue);
		}
	}

	Foliage::Surface *surface()
	{
		return _representation;
	}

	void startMusic()
	{
		Foliage::SoundManager::playBg(&_music);		
		_timer.start();
		#ifndef __PPC__
			SDL_PauseAudio(0); //TEMP: implementation specific
		#endif
	}

	Foliage::Fixed toCloserEvent() const
	{
		const Foliage::Fixed nextEvent = _beats.front().time;
		const Foliage::Fixed toNextEvent = nextEvent - _thisFrame;
		const Foliage::Fixed fromLastEvent = _thisFrame - _lastEvent;
		if (toNextEvent > fromLastEvent)
		{
			return fromLastEvent.opposite();
		}
		else
		{
			return toNextEvent;
		}
	}

	Uint32 events() const
	{
		return _events;
	}

protected:
	Foliage::Fixed    one_frame; //TEMP: pseudo_const

	Foliage::Sound    _music;
	Foliage::Timer    _timer;
	Beats             _beats;
	Foliage::Surface *_representation;
	Foliage::Fixed    _lastEvent;
	Foliage::Fixed    _thisFrame;
	Uint32            _events;
	Sm::SMfile       *_sm;
	Sint32            _measure;

	Sint32   _temp, _prev;
};

#endif //__RYTHM_DISPLAY
