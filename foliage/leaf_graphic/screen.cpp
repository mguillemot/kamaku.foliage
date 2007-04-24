#include <iostream>
#include "screen.hpp"
#include "colors.hpp"

#ifdef __PPC__

#include <xintc_l.h>
#include <xexception_l.h>
#include <xparameters.h>
#include <ivga.h>
#include <isuperdma.h>
#include <isprite.h>

Uint32 Foliage::Screen::_buffer[2] = { 0, 0 };
Uint32 Foliage::Screen::_current_buffer = 0;
Uint32 Foliage::Screen::_black_buffer = 0;
volatile bool Foliage::Screen::_frame_complete = false;
volatile bool Foliage::Screen::_superdma_complete = false;
volatile bool Foliage::Screen::_sprite_complete = false;
Sint32 Foliage::Screen::_last_frame = 0;
Sint32 Foliage::Screen::_current_frame = 0;
Uint32 Foliage::Screen::_sprite_currenly_loaded = 0;
Sint32 Foliage::Screen::_current_instance = -1;

void Foliage::Screen::ivgaInterruptHandler(void *useless)
{
	_frame_complete = true;
	_current_frame++;
}

void Foliage::Screen::isuperdmaInterruptHandler(void *useless)
{
	_superdma_complete = true;
}

void Foliage::Screen::ispriteInterruptHandler(void *useless)
{
	_sprite_complete = true;
}

bool Foliage::Screen::instancizeSurface(const Foliage::Surface *surf, const Foliage::Point pos)
{
	if (_current_instance == 4095)
	{
		return false;
	}
	_current_instance++;
	Sint16 x = SCREEN_WIDTH - surf->getSize().h - pos.y;
	Sint32 y = pos.x;
	ivga_set_instance(_current_instance, surf->getInstancized(), x, y);
	return true;
}

#else

#include <SDL.h>

SDL_Surface *Foliage::Screen::_screen = NULL;
Foliage::Timer Foliage::Screen::_timer;

inline void SDLsetPixel(SDL_Surface *surface, const int x, const int y, const Foliage::Color pixel)
{
	SDL_LockSurface(surface);
	if (surface->format->BytesPerPixel != 2)
	{
		std::cout << "FOLIAGE can only work on 16 bpp surfaces." << std::endl;
		exit(1);
	}
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 2;
	Foliage::Color *pix = (Foliage::Color *)p;
	*pix = pixel;
	SDL_UnlockSurface(surface);
}

#endif

Sint32 Foliage::Screen::waitUntilEndOfFrame()
{
	#ifdef __PPC__
		// Does the real wait
		_frame_complete = false;
		while (!_frame_complete)
		{
		}
		_frame_complete = false;
		Sint32 frameSkipped = _current_frame - _last_frame - 1;
		_last_frame = _current_frame;
		
		// Validate IvgaInstance data
		if (_current_instance != -1)
		{
			ivga_last_instance(_current_instance);
			ivga_validate_instances();
			ivga_activate_instances();
			_current_instance = -1;
		}
		else
		{
			ivga_desactivate_instances();
		}

		return frameSkipped;
	#else
		const Uint32 oneFrame = 1000000 / SCREEN_FPS;
		const Uint32 currentDuration = _timer.duration();
		Sint32 frameSkipped = 0;
		Uint32 basis = 0;
		if (currentDuration > oneFrame)
		{
			frameSkipped = currentDuration / oneFrame;
			basis = frameSkipped * oneFrame;
		}
		while ((_timer.duration() - basis) < oneFrame)
        {
            SDL_Delay(1); 
        }
		_timer.reset();
		_timer.start();
		return frameSkipped;
	#endif
}

void Foliage::Screen::blitSectionSynchronizator()
{
	#ifdef __PPC__
		while (!_superdma_complete)
		{
		}
	#endif
}

Foliage::Synchronizator Foliage::Screen::asyncBlitSection(const Foliage::Surface *surf,	const Foliage::Rect section, const Foliage::Point pos)
{
	#ifdef __PPC__
		_superdma_complete = false;
		const Uint32 sprite_addr = (Uint32)surf->getPixels();
		const Foliage::Size sprite_size = surf->getSize();
		#ifdef REARRANGE_SURFACES
			isuperdma_blit(sprite_addr, sprite_size.h, sprite_size.h - section.y - section.h,
				section.x, section.h, section.w, _current_buffer, SCREEN_WIDTH - pos.y - section.h,
				pos.x);
		#else
			isuperdma_blit(sprite_addr, sprite_size.w, section.x, section.y, section.w,
				section.h, _current_buffer, pos.x, pos.y);
		#endif
	#else
		SDL_Rect srect;
		srect.x = section.x;
		srect.y = section.y;
		srect.w = section.w;
		srect.h = section.h;
		SDL_BlitSurface(surf->getSDLSurface(), &srect, _screen, NULL);
	#endif
	return &Foliage::Screen::blitSectionSynchronizator;
}

void Foliage::Screen::flip()
{
	#ifdef __PPC__
		ivga_set_framebuffer(_current_buffer);
		if (_current_buffer == _buffer[0])
		{
			_current_buffer = _buffer[1];
		}
		else
		{
			_current_buffer = _buffer[0];
		}
	#else
		SDL_Flip(_screen);
	#endif
}

void Foliage::Screen::init()
{
	#ifdef __PPC__
		// Driver init & interrupt handling
		XIntc_RegisterHandler(XPAR_OPB_INTC_0_BASEADDR,
			XPAR_OPB_INTC_0_IVGA_0_VGA_INTERRUPT_INTR,
			(XInterruptHandler)ivgaInterruptHandler,
			NULL);
		XIntc_RegisterHandler(XPAR_OPB_INTC_0_BASEADDR,
			XPAR_OPB_INTC_0_ISUPERDMA_0_ISUPERDMA_INTERRUPT_INTR,
			(XInterruptHandler)isuperdmaInterruptHandler,
			NULL);
		XIntc_RegisterHandler(XPAR_OPB_INTC_0_BASEADDR,
			XPAR_OPB_INTC_0_ISPRITE_0_ISPRITE_INTERRUPT_INTR,
			(XInterruptHandler)ispriteInterruptHandler,
			NULL);
		isuperdma_init(XPAR_ISUPERDMA_0_BASEADDR);
		isprite_init(XPAR_ISPRITE_0_BASEADDR);
		ivga_init(XPAR_IVGA_0_BASEADDR);
		ivga_desactivate_instances();
		ivga_activate_interrupt();

		// Perform buffer init
		_buffer[0] = (Uint32)(new Color[Width * Height]);
		_buffer[1] = (Uint32)(new Color[Width * Height]);
		_current_buffer = _buffer[0];
		_black_buffer = (Uint32)(new Color[Width * Height]);
		ivga_fill_screen(_black_buffer, Foliage::Colors::Black);
		ivga_fill_screen(_buffer[0], Foliage::Colors::Black);
		ivga_fill_screen(_buffer[1], Foliage::Colors::Black);
		flip();
	#else
		_screen = SDL_SetVideoMode(Width, Height, 16, SDL_SWSURFACE | SDL_DOUBLEBUF);
		if (_screen == NULL)
		{
			std::cout << "Couldn't set required video mode." << std::endl;
			exit(1);
		}
		SDL_WM_SetCaption("Kamaku", NULL);
		_timer.start();
	#endif

	// Report init success
	std::cout << " * screen initialized: " << Width << "x" << Height << " ";
	if (Mode == Foliage::YOKO)
	{
		std::cout << "YOKO ";
	}
	else
	{
		std::cout << "TATE ";
	}
	#ifdef __PPC__
		#ifdef SCREEN_VGA
			std::cout << "(31 kHz compatibility)";
		#else
			std::cout << "(15 kHz)";
		#endif
		std::cout << std::endl;
		ivga_on();
	#else
		std::cout << "(window)" << std::endl;
	#endif
}

void Foliage::Screen::clear()
{
	#ifdef __PPC
		_superdma_complete = false;
		isuperdma_blit(_black_buffer, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, _current_buffer, 0, 0);
		while (!_superdma_complete)
		{
		}
	#else
		SDL_FillRect(_screen, NULL, SDL_MapRGB(_screen->format, 0, 0, 0));
	#endif
}

void Foliage::Screen::setPixel(const Foliage::Point pos, const Foliage::Color color)
{
	if (pos.x >= 0 && pos.x < Width && pos.y >= 0 && pos.y < Height)
	{
		#ifdef __PPC__
			#ifdef REARRANGE_SURFACES
				ivga_set_pixel(_current_buffer, Height - 1 - pos.y, pos.x, color);
			#else
				ivga_set_pixel(_current_buffer, pos.x, pos.y, color);
			#endif
		#else
			SDLsetPixel(_screen, pos.x, pos.y, color);
		#endif
	}
}

void Foliage::Screen::fillRect(const Foliage::Rect r, const Foliage::Color color)
{
	#ifdef __PPC__
		for (Sint16 j = r.y; j < r.y + r.h; j++)
		{
			for (Sint16 i = r.x; i < r.x + r.w; i++)
			{
				Foliage::Screen::setPixel(Foliage::Point(i, j), color);
			}
		}
	#else
		SDL_Rect s;
		s.x = r.x;
		s.y = r.y;
		s.h = r.h;
		s.w = r.w;
		SDL_FillRect(_screen, &s, color);
	#endif
}

void Foliage::Screen::blitSurface(const Foliage::Surface *surf, const Foliage::Point pos)
{
	#ifdef __PPC__
		const Uint32 sprite_addr = Uint32(surf->getPixels());
		const Foliage::Size sprite_size = surf->getSize();
		// read...
		if (_sprite_currenly_loaded != sprite_addr)
		{
			_sprite_complete = false;
			#ifdef REARRANGE_SURFACES
				isprite_load_sprite(sprite_addr, sprite_size.h, sprite_size.w);
			#else
				isprite_load_sprite(sprite_addr, sprite_size.w, sprite_size.h);
			#endif
			_sprite_currenly_loaded = sprite_addr;
			while (!_sprite_complete)
			{
			}
		}
		// ...and write
		_sprite_complete = false;
		#ifdef REARRANGE_SURFACES
			isprite_blit_sprite(_current_buffer, SCREEN_WIDTH - pos.y - sprite_size.h, pos.x);
		#else
			isprite_blit_sprite(_current_buffer, pos.x, pos.y);
		#endif
		while (!_sprite_complete)
		{
		}
	#else
		SDL_Rect dest;
		dest.x = pos.x;
		dest.y = pos.y;
		SDL_BlitSurface(surf->getSDLSurface(), NULL, _screen, &dest);
	#endif
}

void Foliage::Screen::drawSurface(const Foliage::Surface *surf, const Foliage::Point pos)
{
	const Foliage::Size s = surf->getSize();
	for (Sint16 j = 0; j < s.h; j++)
	{
		for (Sint16 i = 0; i < s.w; i++)
		{
			
			Foliage::Color color = surf->getPixel(Foliage::Point(i, j));
			if (color != Foliage::Colors::Transparent)
			{
				Foliage::Screen::setPixel(Foliage::Point(pos.x + i, pos.y + j), color);
			}
		}
	}
}
