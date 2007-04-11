#include "timer.hpp"

#ifdef __PPC__
	#include <xparameters.h>
#else
	#include <SDL.h>
#endif

Foliage::Timer::Timer()
	: _started(0), _stopped(0)
{
}

void Foliage::Timer::start()
{
	if (_stopped == 0)
	{
		#ifdef __PPC__
			XTime_GetTime(&_started);
		#else
			_started = SDL_GetTicks();
		#endif
	}
	else
	{
		#ifdef __PPC__
			XTime now;
			XTime_GetTime(&now);
			_started = now - (_stopped - _started);
			_stopped = 0;
		#else
			_started = SDL_GetTicks() - (_stopped - _started);
			_stopped = 0;
		#endif
	}
}

void Foliage::Timer::stop()
{
	#ifdef __PPC__
		XTime_GetTime(&_stopped);
	#else
		_stopped = SDL_GetTicks();
	#endif
}

Uint32 Foliage::Timer::duration() const
{
	if (_started == 0)
	{
		return 0;
	}
	if (_stopped == 0)
	{
		#ifdef __PPC__
			XTime now;
			XTime_GetTime(&now);
			return (now - _started) * 1000000 / XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ;
		#else
			return (SDL_GetTicks() - _started) * 1000;
		#endif
	}
	else
	{
		#ifdef __PPC__
			return (_stopped - _started) * 1000000 / XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ;
		#else
			return (_stopped - _started) * 1000;
		#endif
	}
}

void Foliage::Timer::reset()
{
	_started = 0;
	_stopped = 0;
}
