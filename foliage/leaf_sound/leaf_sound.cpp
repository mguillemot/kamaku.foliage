#include <iostream>
#include <memory>
#include "leaf_sound.hpp"

Foliage::Sound *Foliage::SoundManager::_bg = NULL;
Foliage::Sound *Foliage::SoundManager::_sfx = NULL;

#ifdef __PPC__

#include <xac97_l.h>
#include <xparameters.h>
#include <xintc_l.h>
#include <xexception_l.h>

void Foliage::SoundManager::AC97_Callback(void *useless)
{
	Uint32 sample_sfx, sample_bg, sample_mixed;
	for (Sint32 i = 0; i < 256; i++)
	{
		sample_bg = (_bg != NULL) ? _bg->getNextSample() : 0;
		sample_sfx = (_sfx != NULL) ? _sfx->getNextSample() : 0;
		sample_mixed = mixSamples(sample_bg, sample_sfx);
		XAC97_WriteFifo(XPAR_AUDIO_CODEC_BASEADDR, sample_mixed);
	}
	if (_bg != NULL && _bg->ended())
	{
		_bg = NULL;
	}
	if (_sfx != NULL && _sfx->ended())
	{
		_sfx = NULL;
	}
}

void Foliage::SoundManager::init()
{
	// hard reset & initialization
	XAC97_HardReset(XPAR_AUDIO_CODEC_BASEADDR);
	XAC97_InitAudio(XPAR_AUDIO_CODEC_BASEADDR, AC97_ANALOG_LOOPBACK);
	XAC97_DisableInput(XPAR_AUDIO_CODEC_BASEADDR, AC97_MIC_INPUT);
	XAC97_DisableInput(XPAR_AUDIO_CODEC_BASEADDR, AC97_LINE_INPUT);
	XAC97_AwaitCodecReady(XPAR_AUDIO_CODEC_BASEADDR);
	
	// volume settings
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MasterVol, AC97_VOL_MAX);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_AuxOutVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MasterVolMono, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_PCBeepVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_PhoneInVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MicVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_LineInVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_CDVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_VideoVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_AuxInVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_PCMOutVol, AC97_VOL_MAX);
	XAC97_AwaitCodecReady(XPAR_AUDIO_CODEC_BASEADDR);
	
	// VRA mode OFF
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_ExtendedAudioStat, 0);
	
	// clear FIFOs
	XAC97_ClearFifos(XPAR_AUDIO_CODEC_BASEADDR);

	// interrupt
	XIntc_RegisterHandler(XPAR_OPB_INTC_0_BASEADDR,
		XPAR_OPB_INTC_0_AUDIO_CODEC_INTERRUPT_INTR,
		(XInterruptHandler)Foliage::SoundManager::AC97_Callback,
		NULL);
	XAC97_mSetControl(XPAR_AUDIO_CODEC_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);
	
	std::cout << " * sound manager initialized" << std::endl;
}

void Foliage::SoundManager::disableSound()
{
	XAC97_mSetControl(XPAR_AUDIO_CODEC_BASEADDR, 0);
	std::cout << "LEAF_sound: sound disabled" << std::endl;
}

#else

SDL_AudioSpec Foliage::SoundManager::_audioSpec;
Uint32 mixbuffer[256];

void Foliage::SoundManager::SDL_Audio_Callback(void *userdata, Uint8 *stream, Sint32 len)
{
	if (len != (_audioSpec.samples * 4))
	{
		std::cout << "Invalid audio callback buffer length: " << len << std::endl;
		return;
	}
	Uint32 sample_sfx, sample_bg, sample_mixed;
	Uint32 *buf = (Uint32 *)stream;
	for (Sint32 i = 0; i < _audioSpec.samples; i++)
	{
		sample_bg = (_bg != NULL) ? _bg->getNextSample() : 0;
		sample_sfx = (_sfx != NULL) ? _sfx->getNextSample() : 0;
		sample_mixed = mixSamples(sample_bg, sample_sfx);
		*(buf++) = sample_mixed;
	}
	if (_bg != NULL && _bg->ended())
	{
		_bg = NULL;
	}
	if (_sfx != NULL && _sfx->ended())
	{
		_sfx = NULL;
	}
}

void Foliage::SoundManager::init()
{
	_audioSpec.freq = 48000;
	_audioSpec.format = AUDIO_S16SYS;
	_audioSpec.channels = 2;
	_audioSpec.samples = 2048;
	_audioSpec.callback = SDL_Audio_Callback;
	_audioSpec.userdata = NULL;
	SDL_OpenAudio(&_audioSpec, NULL);
	std::cout << " * sound manager initialized" << std::endl;
}

void Foliage::SoundManager::disableSound()
{
	SDL_CloseAudio();
	std::cout << "LEAF_sound: sound disabled" << std::endl;
}

#endif

Uint32 Foliage::SoundManager::mixSamples(const Uint32 sample_bg, const Uint32 sample_sfx)
{
	Sint32 g1 = (Sint32)((Sint16)(sample_bg >> 16));
	Sint32 d1 = (Sint32)((Sint16)(sample_bg & 0xffff));
	Sint32 g2 = (Sint32)((Sint16)(sample_sfx >> 16));
	Sint32 d2 = (Sint32)((Sint16)(sample_sfx & 0xffff));
	Sint32 g = (g1 + g2) >> 1;
	Sint32 d = (d1 + d2) >> 1;
	/* (no bound check needed since samples are /2)
	if (g > 32767)
	{
		g = 32767;
	}
	else if (g < -32768)
	{
		g = -32768;
	}
	if (d > 32767)
	{
		d = 32767;
	}
	else if (d < -32768)
	{
		d = -32768;
	}
	*/
	Uint32 sample_mixed = ((Uint32)g << 16);
	sample_mixed |= ((Uint32)d & 0xffff);
	return sample_mixed;
}

void Foliage::SoundManager::playBg(Foliage::Sound *bg)
{
	bg->rewind();
	_bg = bg;
}

void Foliage::SoundManager::playSfx(Foliage::Sound *sfx)
{
	sfx->rewind();
	_sfx = sfx;
}
