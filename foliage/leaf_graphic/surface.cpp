#include <iostream>
#include "../foliage.hpp"
#include "../settings.hpp"
#include "../memory.hpp"
#include "surface.hpp"
#include "screen.hpp"
#include "colors.hpp"

#ifdef __PPC__
	#include <sysace_stdio.h>
	#include <ivga.h>
	#define FILE SYSACE_FILE
	#define fopen sysace_fopen
	#define fread sysace_fread
	#define fclose sysace_fclose
#else

#include <cstdio>
#include <SDL.h>

void Foliage::Surface::lock() const
{
	if (_locks == 0)
	{
		SDL_LockSurface(_SDLSurface);
	}
	_locks++;
}

void Foliage::Surface::unlock() const
{
	_locks--;
	if (_locks == 0)
	{
		SDL_UnlockSurface(_SDLSurface);
	}
}

#endif

Foliage::Surface::Surface(const Foliage::Size s, Foliage::Color *pixels, const std::string &name) 
	: _size(s)
{
	_name = name;
	#ifdef __PPC__
		_pixels = pixels;
		_instancized = -1;
	#else
		_SDLSurface = SDL_CreateRGBSurfaceFrom(pixels, s.w, s.h, 16, s.w * 2, 0x1f << 11, 0x3f << 5, 0x1f, 0);
		_locks = 0;
        SDL_SetColorKey(_SDLSurface, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(_SDLSurface->format, 0xFF, 0, 0xFF));	
	#endif
}

Foliage::Surface *Foliage::Surface::createSurface(const Foliage::Size s, const std::string &name)
{
	Foliage::Color *pixels = new Color[s.w * s.h];
	return new Foliage::Surface(s, pixels, name);
}

Foliage::Surface::~Surface()
{
	#ifdef __PPC__
		delete _pixels;
	#else
		SDL_FreeSurface(_SDLSurface);
	#endif
}

Foliage::Color Foliage::Surface::getPixel(const Foliage::Point p) const
{
	#ifdef __PPC__
		#ifdef REARRANGE_SURFACES
			return _pixels[p.x * _size.h + _size.h - 1 - p.y];
		#else
			return _pixels[p.y * _size.w + p.x];
		#endif
	#else
		lock();
		if (_SDLSurface->format->BytesPerPixel != 2)
		{
			std::cout << "FOLIAGE can only work on 16 bpp surfaces." << std::endl;
			exit(1);
		}
		Uint8 *addr = (Uint8 *)_SDLSurface->pixels + p.y * _SDLSurface->pitch + p.x * 2;
		Foliage::Color *pix = (Foliage::Color *)addr;
		unlock();
		return *pix;
	#endif
}

Foliage::Rect Foliage::Surface::getBoundingRect() const
{
	return Foliage::Rect(Foliage::Point(0, 0), _size);
}

void Foliage::Surface::setPixel(const Foliage::Point p, const Foliage::Color color)
{
	#ifdef __PPC__
		#ifdef REARRANGE_SURFACES
			_pixels[p.x * _size.h + _size.h - 1 - p.y] = color;
		#else
			_pixels[p.y * _size.w + p.x] = color;
		#endif
	#else
		lock();
		Uint8 *addr = (Uint8 *)_SDLSurface->pixels + p.y * _SDLSurface->pitch + p.x * 2;
		Foliage::Color *pix = (Foliage::Color *)addr;
		*pix = color;
		unlock();
	#endif
}

void Foliage::Surface::drawLine(const Foliage::Point from, const Foliage::Point to, const Foliage::Color color)
{
	#ifndef __PPC__
		lock();
	#endif
	if (from.x == to.x)
	{
		// vertical line
		for (Sint16 y = from.y; y <= to.y; y++)
		{
			setPixel(Foliage::Point(from.x, y), color);
		}
	}
	else if (from.y == to.y)
	{
		// horizontal line
		for (Sint16 x = from.x; x <= to.x; x++)
		{
			setPixel(Foliage::Point(x, from.y), color);
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
			const Foliage::Fixed a = Foliage::Fixed(dy) / Foliage::Fixed(dx);
			const Foliage::Fixed b = Foliage::Fixed(from.y) - a * from.x;
			Foliage::Fixed y = a * from.x + b;
			setPixel(Foliage::Point(from.x, y), color);
			for (Sint16 x = from.x + 1; x <= to.x; x++)
			{
				y += a;
				setPixel(Foliage::Point(x, y), color);
			}
		}
		else
		{
			// x = a'*y + b'
			const Foliage::Fixed a_prime = Foliage::Fixed(dx) / Foliage::Fixed(dy);
			const Foliage::Fixed b_prime = Foliage::Fixed(from.x) - a_prime * from.y;
			Foliage::Fixed x = a_prime * from.y + b_prime;
			setPixel(Foliage::Point(x, from.y), color);
			for (Sint16 y = from.y + 1; y <= to.y; y++)
			{
				x += a_prime;
				setPixel(Foliage::Point(x, y), color);
			}
		}
	}
	#ifndef __PPC__
		unlock();
	#endif
}

void Foliage::Surface::drawRect(const Foliage::Rect &r, const Foliage::Color color)
{
	#ifndef __PPC__
		lock();
	#endif
	const Foliage::Point c1 = Foliage::Point(r.x, r.y);
	const Foliage::Point c2 = Foliage::Point(r.x, r.y + r.h - 1);
	const Foliage::Point c3 = Foliage::Point(r.x + r.w - 1, r.y);
	const Foliage::Point c4 = Foliage::Point(r.x + r.w - 1, r.y + r.h - 1);
	drawLine(c1, c2, color);
	drawLine(c1, c3, color);
	drawLine(c2, c4, color);
	drawLine(c3, c4, color);
	#ifndef __PPC__
		unlock();
	#endif
}

void Foliage::Surface::fill(const Foliage::Color color)
{
	#ifdef __PPC__
		for (int i = 0; i < (_size.w * _size.h); i++)
		{
			_pixels[i] = color;
		}
	#else
		SDL_FillRect(_SDLSurface, NULL, color);
	#endif
}

void Foliage::Surface::drawAt(const Foliage::Point p) const
{
	#ifdef __PPC__
		if (_instancized != -1)
		{
			// try to use ivga
			bool ok = Foliage::Screen::instancizeSurface(this, p);
			if (!ok)
			{
				// use isprite
				Foliage::Screen::blitSurface(this, p);
			}
		}
		else if (_size.h <= 64 && _size.w <= 64 && p.x >= 0)
		{
			// use isprite
			Foliage::Screen::blitSurface(this, p);
		}
		else
		{
			// draw manually (sloooooow!)
			Foliage::Screen::drawSurface(this, p);
		}
	#else
		Foliage::Screen::blitSurface(this, p);
	#endif
}

typedef struct BMP_HEADER
{
   Uint16 type;                     // Magic identifier
   Uint32 size;                     // File size in bytes
   Uint16 reserved1, reserved2;     // --
   Uint32 offset;                   // Offset to image data, bytes
} HEADER;

typedef struct BMP_INFOHEADER
{
   Uint32 size;                     // Header size in bytes
   Sint32 width, height;            // Width and height of image
   Uint16 planes;                   // Number of color planes
   Uint16 bits;                     // Bits per pixel
   Uint32 compression;              // Compression type
   Uint32 imagesize;                // Image size in bytes
   Sint32 xresolution, yresolution; // Pixels per meter
   Uint32 ncolors;                  // Number of colors
   Uint32 importantcolors;          // Important colors
} INFOHEADER;

// reads two bytes from file
inline Uint16 ReadUShort(FILE *fptr)
{
	Uint8 readBuffer[2];
	fread(readBuffer, 1, 2, fptr);
	return ((readBuffer[1] << 8) | readBuffer[0]);
}

// reads four bytes from file in little endian order
inline Uint32 ReadUIntLil(FILE *fptr)
{
	Uint8 readBuffer[4];	
	fread(readBuffer, 1, 4, fptr);
	return ((((((readBuffer[3] << 8) | readBuffer[2]) << 8) | readBuffer[1]) << 8) | readBuffer[0]);
}

inline Foliage::Color create_color_from_truecolor(Uint32 truecolor)
{
	Uint32 r = (truecolor >> 16) & 0xff;
	Uint32 g = (truecolor >> 8) & 0xff;
	Uint32 b = truecolor & 0xff;
	r >>= 3;
	g >>= 2;
	b >>= 3;
	Foliage::Color color = (r << 11) | (g << 5) | b;
	return color;
}

inline void PadToNextMultipleOfFour(Sint32 &val)
{
	if (val % 4 == 1)
	{
		val += 3;
	}
	else if (val % 4 == 2)
	{
		val += 2;
	}
	else if (val % 4 == 3)
	{
		val++;
	}
}

Foliage::Surface *Foliage::Surface::readBMP(const std::string &filename)
{
	FILE *infile;
	#ifdef __PPC__
		infile = fopen(filename.c_str(), "r");
	#else
		std::string filename2 = "../game/resources/" + filename;
		infile = fopen(filename2.c_str(), "rb");
	#endif

	if (infile == NULL)
	{
		std::cout << "Could not open file " << filename << std::endl;
		exit(1);
	}
	
	// Read check the headers
	HEADER header;
	INFOHEADER infoheader;
	header.type = ReadUShort(infile);
	header.size = ReadUIntLil(infile);
	header.reserved1 = ReadUShort(infile);
	header.reserved2 = ReadUShort(infile);
	header.offset = ReadUIntLil(infile);
	infoheader.size = ReadUIntLil(infile);
	infoheader.width = ReadUIntLil(infile);
	infoheader.height = ReadUIntLil(infile);
	infoheader.planes = ReadUShort(infile);
	infoheader.bits = ReadUShort(infile);
	infoheader.compression = ReadUIntLil(infile);
	infoheader.imagesize = ReadUIntLil(infile);
	infoheader.xresolution = ReadUIntLil(infile);
	infoheader.yresolution = ReadUIntLil(infile);
	infoheader.ncolors = ReadUIntLil(infile);
	infoheader.importantcolors = ReadUIntLil(infile);
	
	// Check maximum buffer width
	if (infoheader.width > 1024)
	{
		std::cout << "Cannot read BMP with width > 1024." << std::endl;
		exit(1);
	}
	
	// Compute line (or column) padding on 4 pixels (due to 64-bit PLB)
	Sint32 width = infoheader.width;
	Sint32 height = infoheader.height;
	#ifdef REARRANGE_SURFACES
		PadToNextMultipleOfFour(height);
	#else
		PadToNextMultipleOfFour(width);
	#endif

	// Allocate surface memory
	Color *baseaddr = new(Foliage::Eternal) Color[height * width];
	if (baseaddr == NULL)
	{
		std::cout << "Not enough memory." << std::endl;
		fclose(infile);
		exit(1);
	}

	// Read raw 24-bit data, transform them to system colors, and write them in memory
	Uint8 readBuffer[1024*3];
	Sint32 linewidth = infoheader.width * 3;
	PadToNextMultipleOfFour(linewidth);
	for (Sint32 j = infoheader.height - 1; j >= 0; j--)
	{
		size_t r = fread(readBuffer, 1, linewidth, infile);
		if (r != linewidth)
		{
			std::cout << "Corrupt file: " << filename << std::endl;
			exit(1);
		}
		for (Sint32 i = 0; i < infoheader.width; i++)
		{
			Uint32 temp = ((((readBuffer[i * 3 + 2] << 8) | readBuffer[i * 3 + 1]) << 8) | readBuffer[i * 3]);
			Color *writeaddr = baseaddr;
			#ifdef REARRANGE_SURFACES
				// If TATE mode, transform the line data to column data
				writeaddr += (i * height + (height - 1 - j));
			#else
				writeaddr += (j * width + i);
			#endif
			*writeaddr = create_color_from_truecolor(temp);
		}
	}
	fclose(infile);
	
	// Add transparent padding
	#ifdef REARRANGE_SURFACES
		for (Sint32 j = infoheader.height; j < height; j++)
		{
			for (Sint32 i = 0; i < infoheader.width; i++)
			{
				Color *writeaddr = baseaddr;
				writeaddr += (i * height + (height - 1 - j));
				*writeaddr = Foliage::Colors::Transparent;
			}
		}
	#else
		for (Sint32 j = 0; j < height; j++)
		{
			for (Sint32 i = infoheader.width; i < width; i++)
			{
				Color *writeaddr = baseaddr;
				writeaddr += (j * width + i);
				*writeaddr = Foliage::Colors::Transparent;
			}
		}
	#endif
		
	// Flush processor cache
	Foliage::flushDCache();

	// Allocates and create the surface
	return new(Foliage::Eternal) Foliage::Surface(Foliage::Size(width, height), baseaddr, filename);
}

#ifdef __PPC__

Foliage::Surface *Foliage::Surface::createNewShiftedSurface(const Sint32 shift) const
{
	// Allocate surface memory
	Foliage::Color *baseaddr = new(Foliage::Eternal) Foliage::Color[_size.w * _size.h];
	if (baseaddr == NULL)
	{
		std::cout << "Not enough memory." << std::endl;
		return NULL;
	}
	
	// Compute the shifted surface
	#ifdef REARRANGE_SURFACES
		for (Sint32 j = shift; j < _size.h; j++)
		{
			for (Sint32 i = 0; i < _size.w; i++)
			{
				Foliage::Color *writeaddr = baseaddr;
				writeaddr += (i * _size.h + (_size.h - 1 - j));
				*writeaddr = _pixels[(i * _size.h + (_size.h - 1 - (j - shift)))];
			}
		}
	#else
		for (Sint32 j = 0; j < _size.h; j++)
		{
			for (Sint32 i = shift; i < _size.w; i++)
			{
				Foliage::Color *writeaddr = baseaddr;
				writeaddr += (j * width + i);
				*writeaddr = _pixels[(j * _size.w + (i - shift))];
			}
		}
	#endif
	
	// Flush processor cache
	Foliage::flushDCache();
	
	// Allocates and create the surface
	std::string newname = _name;
	newname += '+';
	newname += ('0' + shift);
	return new(Foliage::Eternal) Foliage::Surface(_size, baseaddr, newname);
}

#endif
