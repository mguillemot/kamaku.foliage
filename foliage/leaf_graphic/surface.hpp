#ifndef _FOLIAGE__SURFACE
#define _FOLIAGE__SURFACE

#include <string>
#include <list>
#include <vector>
#include "../basic_types.hpp"
#include "graphic_types.hpp"
#include "colors.hpp"
#include "screen.hpp"

#ifndef __PPC__
	#include <SDL.h>
#endif

namespace Foliage
{

	class Surface
	{
	public:
		Surface(const Size s, const std::string &name)
			: _size(s), _name(name)
		{
			#ifdef __PPC__
				_pixels = new Color[s.w * s.h];
				if (_pixels == NULL)
				{
					std::cout << "ERROR: not enough memory to allocate surface." << std::endl;
					exit(1);
				}
				_instancized = -1;
			#else
				_sdlSurface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY, s.w, s.h, 16, 0x1f << 11, 0x3f << 5, 0x1f, 0);
				_locks = 0;
				if (_sdlSurface->format->BytesPerPixel != 2)
				{
					std::cout << "SDL_ERROR: not a 16 bpp surface." << std::endl;
					exit(1);
				}
				SDL_SetColorKey(_sdlSurface, SDL_SRCCOLORKEY, Colors::Transparent);	
			#endif
		}

		~Surface()
		{
			#ifdef __PPC__
				delete _pixels;
			#else
				SDL_FreeSurface(_sdlSurface);
			#endif
		}
		
		Color getPixel(const Point p) const
		{
			#ifdef __PPC__
				#ifdef REARRANGE_SURFACES
					return _pixels[p.x * _size.h + _size.h - 1 - p.y];
				#else
					return _pixels[p.y * _size.w + p.x];
				#endif
			#else
				lock();
				Uint8* addr = (Uint8*)_sdlSurface->pixels + p.y * _sdlSurface->pitch + p.x * 2;
				Color* pix = (Color*)addr;
				Color c = *pix;
				unlock();
				return c;
			#endif
		}

		void setPixel(const Point p, const Color color)
		{
			#ifdef __PPC__
				#ifdef REARRANGE_SURFACES
					_pixels[p.x * _size.h + _size.h - 1 - p.y] = color;
				#else
					_pixels[p.y * _size.w + p.x] = color;
				#endif
			#else
				lock();
				Uint8* addr = (Uint8*)_sdlSurface->pixels + p.y * _sdlSurface->pitch + p.x * 2;
				Color* pix = (Color*)addr;
				*pix = color;
				unlock();
			#endif
		}

		void fill(const Color color)
		{
			#ifdef __PPC__
				Color* pixelsEnd = _pixels;
				pixelsEnd += _size.w * _size.h;
				const Uint32* end = (Uint32*)pixelsEnd;
				const Uint32 doubleColor = ((Uint32)color << 16) | (Uint32)color;
				for (Uint32* ptr = (Uint32*)_pixels; ptr < end; ptr++)
				{
					*(ptr++) = doubleColor;
				}
			#else
				SDL_FillRect(_sdlSurface, NULL, color);
			#endif
		}

		void drawLine(const Point from, const Point to, const Color color)
		{
			#ifndef __PPC__
				lock();
			#endif
			if (from.x == to.x)
			{
				// vertical line
				for (Sint16 y = from.y; y <= to.y; y++)
				{
					setPixel(Point(from.x, y), color);
				}
			}
			else if (from.y == to.y)
			{
				// horizontal line
				for (Sint16 x = from.x; x <= to.x; x++)
				{
					setPixel(Point(x, from.y), color);
				}
			}
			else
			{
				const Sint16 dx = to.x - from.x;
				const Sint16 dy = to.y - from.y;
				if ((dy + dx) < 0)
				{
					drawLine(to, from, color);
				}
				if (dx >= dy)
				{
					// y = a*b + b
					const Fixed a = Fixed(dy) / Fixed(dx);
					const Fixed b = Fixed(from.y) - a * from.x;
					Fixed y = a * from.x + b;
					setPixel(Point(from.x, y), color);
					for (Sint16 x = from.x + 1; x <= to.x; x++)
					{
						y += a;
						setPixel(Point(x, y), color);
					}
				}
				else
				{
					// x = a'*y + b'
					const Fixed a_prime = Fixed(dx) / Fixed(dy);
					const Fixed b_prime = Fixed(from.x) - a_prime * from.y;
					Fixed x = a_prime * from.y + b_prime;
					setPixel(Point(x, from.y), color);
					for (Sint16 y = from.y + 1; y <= to.y; y++)
					{
						x += a_prime;
						setPixel(Point(x, y), color);
					}
				}
			}
			#ifndef __PPC__
				unlock();
			#endif
		}

		void drawRect(const Rect &r, const Color color)
		{
			#ifndef __PPC__
				lock();
			#endif
			const Point c1 = Point(r.x, r.y);
			const Point c2 = Point(r.x, r.y + r.h - 1);
			const Point c3 = Point(r.x + r.w - 1, r.y);
			const Point c4 = Point(r.x + r.w - 1, r.y + r.h - 1);
			drawLine(c1, c2, color);
			drawLine(c1, c3, color);
			drawLine(c2, c4, color);
			drawLine(c3, c4, color);
			#ifndef __PPC__
				unlock();
			#endif
		}

		void fillRect(const Rect &r, const Color color)
		{
			#ifndef __PPC__
				lock();
			#endif
			for (Sint16 j = r.y; j < r.y + r.h; j++)
			{
				for (Sint16 i = r.x; i < r.x + r.w; i++)
				{
					setPixel(Point(i, j), color);
				}
			}
			#ifndef __PPC__
				unlock();
			#endif
		}

		void drawAt(const Point p) const
		{
			#ifdef __PPC__
				if (_instancized != -1)
				{
					// try to use ivga
					bool ok = Screen::instancizeSurface(this, p);
					if (!ok)
					{
						// use isprite
						Screen::blitSurface(this, p);
					}
				}
				else if (_size.h <= 64 && _size.w <= 64 && p.x >= 0) // HACK: isprite problem if p.x < 0
				{
					// use isprite
					Screen::blitSurface(this, p);
				}
				else
				{
					// draw manually (sloooooow!)
					Screen::drawSurface(this, p);
				}
			#else
				Screen::blitSurface(this, p);
			#endif
		}

		Size getSize() const
		{ 
			return _size; 
		}

		const std::string& getName() const
		{
			return _name; 
		}
		
		Rect getBoundingRect() const
		{
			return Rect(Point(0, 0), _size);
		}

		Surface* createNewShiftedSurface(const Sint32 shift) const
		{
			// Create surface
			std::string newname = _name;
			newname += '+';
			newname += ('0' + shift);
			Surface* s = new Surface(_size, newname);

			// Compute the shifted surface
			Point p;
			for (p.y = 0; p.y < _size.h; p.y++)
			{
				for (p.x = shift; p.x < _size.w; p.x++)
				{
					s->setPixel(Point(p.x, p.y + shift), getPixel(p));
				}
			}
				
			// Return resulting surface
			return s;
		}

		Surface* createNewRotatedSurface(const Sint32 angle) const
		{
			// Check angle validity
			if (angle != 90 && angle != 180 && angle != 270)
			{
				std::cout << "Cannot rotate the surface " << angle << " degrees." << std::endl;
				return NULL;
			}

			// Create surface
			std::string newname = _name;
			newname += '/';
			newname += angle;
			const Size size = (angle == 180) ? _size : _size.inverted();
			Surface* s = new Surface(size, newname);
			
			// Compute the rotated surface
			Point p;
			for (p.y = 0; p.y < _size.h; p.y++)
			{
				for (p.x = 0; p.x < _size.w; p.x++)
				{
					if (angle == 90)
					{
						s->setPixel(Point(_size.h - 1 - p.y, p.x), getPixel(p));
					}
					else if (angle == 180)
					{
						s->setPixel(Point(_size.w - 1 - p.x, _size.h - 1 - p.y), getPixel(p));
					}
					else
					{
						s->setPixel(Point(p.y, _size.w - 1 - p.x), getPixel(p));
					}
				}
			}

			// Return resulting surface
			return s;
		}

		#ifdef __PPC__
			Color* getPixels() const 
			{ 
				return _pixels; 
			}

			void setInstancized(const Sint32 instancized) 
			{ 
				_instancized = instancized; 
			}

			Sint32 getInstancized() const 
			{ 
				return _instancized; 
			}
		#else
			void lock() const
			{
				if (_locks == 0)
				{
					SDL_LockSurface(_sdlSurface);
				}
				_locks++;
			}

			void unlock() const
			{
				_locks--;
				if (_locks == 0)
				{
					SDL_UnlockSurface(_sdlSurface);
				}
			}

			SDL_Surface* getSdlSurface() const
			{ 
				return _sdlSurface; 
			}
		#endif

	private:
		Size        _size;
		std::string _name;
		#ifdef __PPC__
			Color* _pixels;
			Sint32 _instancized;
		#else
			SDL_Surface*   _sdlSurface;
			mutable Sint32 _locks;
		#endif
	};
	
	typedef std::list<Surface *> ListSurface;
	typedef std::list<Surface *> VectorSurface;

}

#endif //_FOLIAGE__SURFACE
