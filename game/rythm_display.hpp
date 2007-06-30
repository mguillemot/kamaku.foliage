#ifndef __RYTHM_DISPLAY
#define __RYTHM_DISPLAY

#include <iostream>
#include <deque>
#include "foliage.hpp"
#include "sm.hpp"

#define RYTHMDISPLAY_WIDTH  80
#define RYTHMDISPLAY_HEIGHT 100

struct Beat
{
	Foliage::Fixed time;
	bool tap[8];
	Foliage::Fixed hold[8];
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
		for (Sint32 k = 0; k < 8; k++)
		{
			_tap[k] = false;
			_hold[k] = F_0;
		}
		std::cout << _sm->level[0].measure_nb << " measures." << std::endl;
		_representation = new Foliage::Surface(Foliage::Size(RYTHMDISPLAY_WIDTH, RYTHMDISPLAY_HEIGHT), "rythm");
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
				bool beatActive = false;
				for (Sint32 k = 0; k < 8; k++)
				{
					
					if (_sm->level[0].measure[i].notes[j].tap[k] == Sm::TAP)
					{
						b.tap[k] = true;
						beatActive = true;
					}
					else
					{
						b.tap[k] = false;
					}
					if (_sm->level[0].measure[i].notes[j].tap[k] == Sm::BEGIN_HOLD)
					{
						Foliage::Fixed duration;
						// search for corresponding END_HOLD
						Sint32 i2 = i;
						Sint32 j2 = j;
						while (_sm->level[0].measure[i2].notes[j2].tap[k] != Sm::END_HOLD)
						{
							duration += measure_length / Sint16(_sm->level[0].measure[i2].notes_nb);
							j2++;
							if (j2 == _sm->level[0].measure[i2].notes_nb)
							{
								j2 = 0; 
								i2++;
							}
						}
						// END_HOLD at measure i2, note j2
						b.hold[k] = duration;						
						beatActive = true;
					}
					else
					{
						b.hold[k] = F_0;
					}
				}
				if (beatActive)
				{
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
		for (Sint32 k = 0; k < 8; k++)
		{
			_tap[k] = false;
			if (_hold[k] > F_0)
			{
				_hold[k] -= one_frame;
			}
		}
		_thisFrame = Foliage::Fixed(Sint16(_timer.duration() / 1000));
		Beats::iterator i = _beats.begin();
		while (i != _beats.end())
		{
			Foliage::Fixed fdelay = i->time - _thisFrame;
			Sint16 delay = Sint16(fdelay / one_frame);
			if (delay < 0)
			{
				_temp--;
				for (Sint32 k = 0; k < 8; k++)
				{
					_tap[k] = i->tap[k];
					if (i->hold[k] > F_0)
					{
						_hold[k] = i->hold[k];
					}
				}
				_lastEvent = i->time;
				i = _beats.erase(i);
			}
			else
			{
				break;
			}
		}

		// Update representation
		_representation->fill(Foliage::Colors::Transparent);
		for (Sint32 k = 0; k < 8; k++)
		{
			_representation->drawLine(Foliage::Point((RYTHMDISPLAY_WIDTH / 8) * k + (RYTHMDISPLAY_WIDTH / 16), 0), Foliage::Point((RYTHMDISPLAY_WIDTH / 8) * k + (RYTHMDISPLAY_WIDTH / 16), RYTHMDISPLAY_HEIGHT - 1), Foliage::Colors::White);
		}
		_representation->drawRect(Foliage::Rect(0, 0, RYTHMDISPLAY_WIDTH, RYTHMDISPLAY_HEIGHT), Foliage::Colors::Blue);
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
				for (Sint32 k = 0; k < 8; k++)
				{
					if (i->tap[k])
					{
						_representation->drawLine(Foliage::Point((RYTHMDISPLAY_WIDTH / 8) * k + (RYTHMDISPLAY_WIDTH / 16) - 3, delay), Foliage::Point((RYTHMDISPLAY_WIDTH / 8) * k + (RYTHMDISPLAY_WIDTH / 16) + 3, delay), Foliage::Colors::Black);
					}
					if (i->hold[k] > F_0)
					{
						Foliage::Fixed holdEnd = fdelay + i->hold[k];
						Sint16 endDelay = Sint16(holdEnd / one_frame);
						endDelay = RYTHMDISPLAY_HEIGHT - endDelay - 1;
						if (endDelay < 0)
						{
							endDelay = 0;
						}
						Foliage::Rect r(Foliage::Point((RYTHMDISPLAY_WIDTH / 8) * k + (RYTHMDISPLAY_WIDTH / 16) - 3, endDelay), Foliage::Size(7, delay - endDelay));
						_representation->fillRect(r, Foliage::Colors::Black);
					}
				}
				++i;
			}
			else
			{
				break;
			}
		}
		for (Sint32 k = 0; k < 8; k++)
		{
			if (_hold[k] > F_0)
			{
				Sint16 endDelay = Sint16(_hold[k] / one_frame);
				endDelay = RYTHMDISPLAY_HEIGHT - endDelay - 1;
				if (endDelay < 0)
				{
					endDelay = 0;
				}
				Foliage::Rect r(Foliage::Point((RYTHMDISPLAY_WIDTH / 8) * k + (RYTHMDISPLAY_WIDTH / 16) - 3, endDelay), Foliage::Size(7, RYTHMDISPLAY_HEIGHT - 1 - endDelay));
				_representation->fillRect(r, Foliage::Colors::Black);
			}			
		}
		if (tap(0))
		{
			_representation->fillRect(Foliage::Rect(0, 0, RYTHMDISPLAY_WIDTH, RYTHMDISPLAY_HEIGHT), Foliage::Colors::Red);
		}
		else if (tap(1))
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

	bool tap(const Sint32 k) const
	{
		return _tap[k];
	}

	bool hold(const Sint32 k) const
	{
		return (_hold[k] > F_0);
	}

protected:
	Foliage::Fixed    one_frame; //TEMP: pseudo_const

	Foliage::Sound    _music;
	Foliage::Timer    _timer;
	Beats             _beats;
	Foliage::Surface *_representation;
	Foliage::Fixed    _lastEvent;
	Foliage::Fixed    _thisFrame;
	bool              _tap[8];
	Foliage::Fixed    _hold[8];
	Sm::SMfile       *_sm;
	Sint32            _measure;

	Sint32   _temp, _prev;
};

#endif //__RYTHM_DISPLAY
