#ifndef _FOLIAGE__LEAF_SOUND
#define _FOLIAGE__LEAF_SOUND

#include <string>
#include "../basic_types.hpp"
#include "sound.hpp"

#ifndef __PPC__
	#include <SDL.h>
#endif

namespace Foliage
{
	
	class SoundManager
	{
	public:
		static void   init();
		static void   disableSound();
		static void   playBg(Foliage::Sound *bg);
		static void   playSfx(Foliage::Sound *sfx);
		static Uint32 mixSamples(const Uint32 sample_bg, const Uint32 sample_sfx);
		
	private:
		SoundManager(){}
		
		static Foliage::Sound *_bg;
		static Foliage::Sound *_sfx;

		#ifdef __PPC__
			static void AC97_Callback(void *useless);
		#else
			static void SDL_Audio_Callback(void *userdata, Uint8 *stream, Sint32 len);

			static SDL_AudioSpec _audioSpec;
		#endif
	};
	
}

#endif //_FOLIAGE__LEAF_SOUND
