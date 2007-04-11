#ifndef _FOLIAGE__TIMER
#define _FOLIAGE__TIMER

#include "basic_types.hpp"

#ifdef __PPC__
	#include <xtime_l.h>
#else
	#include <SDL.h>
#endif

namespace Foliage
{

	class Timer
	{
	public:
		Timer();
		void start();
		void stop();
		void reset();
		Uint32 duration() const; // in microseconds
		
	private:
		#ifdef __PPC__
			XTime _started, _stopped;
		#else
			Uint32 _started, _stopped;
		#endif
	};	
	
}

#endif //_FOLIAGE__TIMER
