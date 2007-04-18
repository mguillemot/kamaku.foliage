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
#endif

Foliage::Surface::Surface(const Foliage::Size s, Foliage::Color *pixels, const std::string &name) 
	: _size(s)
{
	_name = name;
	_pixels = pixels;
	#ifdef __PPC__
		_instancized = -1;
	#else
		_SDLSurface = SDL_CreateRGBSurfaceFrom(pixels, s.w, s.h, 16, s.w * 2, 0x1f << 11, 0x3f << 5, 0x1f, 0);
        SDL_SetColorKey(_SDLSurface, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(_SDLSurface->format, 0xFF, 0, 0xFF));	
	#endif
}

Foliage::Surface::~Surface()
{
	#ifndef __PPC__
		SDL_FreeSurface(_SDLSurface);
	#endif
	delete _pixels;
}

Foliage::Color Foliage::Surface::getPixel(const Foliage::Point p) const
{
	#ifdef REARRANGE_SURFACES
		return _pixels[p.x * _size.h + _size.h - 1 - p.y];
	#else
		return _pixels[p.y * _size.w + p.x];
	#endif
}

void Foliage::Surface::setPixel(const Foliage::Point p, const Foliage::Color color)
{
	#ifdef REARRANGE_SURFACES
		_pixels[p.x * _size.h + _size.h - 1 - p.y] = color;
	#else
		_pixels[p.y * _size.w + p.x] = color;
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
