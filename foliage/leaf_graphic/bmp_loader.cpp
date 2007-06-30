#include <iostream>
#include "bmp_loader.hpp"
#include "../leaf_io/file.hpp"

Foliage::SurfaceDictionary Foliage::BmpLoader::surfaces;

struct BmpHeader
{
   Uint16 type;                     // Magic identifier
   Uint32 size;                     // File size in bytes
   Uint16 reserved1, reserved2;     // --
   Uint32 offset;                   // Offset to image data, bytes
};

struct BmpInfoHeader
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
};

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

Foliage::Surface* performLoadBmp(const std::string& filename) 
{
	Foliage::File infile(filename);
	
	// Read check the headers
	BmpHeader header;
	BmpInfoHeader infoheader;
	header.type = infile.readUint16();
	header.size = infile.readUint32LE();
	header.reserved1 = infile.readUint16();
	header.reserved2 = infile.readUint16();
	header.offset = infile.readUint32LE();
	infoheader.size = infile.readUint32LE();
	infoheader.width = infile.readUint32LE();
	infoheader.height = infile.readUint32LE();
	infoheader.planes = infile.readUint16();
	infoheader.bits = infile.readUint16();
	infoheader.compression = infile.readUint32LE();
	infoheader.imagesize = infile.readUint32LE();
	infoheader.xresolution = infile.readUint32LE();
	infoheader.yresolution = infile.readUint32LE();
	infoheader.ncolors = infile.readUint32LE();
	infoheader.importantcolors = infile.readUint32LE();
	
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
	Foliage::Surface* s = new Foliage::Surface(Foliage::Size((Sint16)width, (Sint16)height), filename);

	// Read raw 24-bit data, transform them to system colors, and write them in memory
	Uint8 readBuffer[1024 * 3];
	Sint32 linewidth = infoheader.width * 3;
	PadToNextMultipleOfFour(linewidth);
	Foliage::Point p;
	for (p.y = infoheader.height - 1; p.y >= 0; p.y--)
	{
		Sint32 r = (Sint32)infile.read(readBuffer, linewidth);
		Uint8* ptr = readBuffer;
		if (r != linewidth)
		{
			std::cout << "Corrupt file: " << filename << std::endl;
			exit(1);
		}
		for (p.x = 0; p.x < infoheader.width; p.x++)
		{
			const Uint32 temp = (ptr[2] << 16) | (ptr[1] << 8) | ptr[0];
			ptr += 3;
			s->setPixel(p, create_color_from_truecolor(temp));
		}
	}
	
	// Add transparent padding
	#ifdef REARRANGE_SURFACES
		for (p.y = infoheader.height; p.y < height; p.y++)
		{
			for (Sint32 p.x = 0; p.x < infoheader.width; p.x++)
			{
				s->setPixel(p, Foliage::Colors::Transparent);
			}
		}
	#else
		for (p.y = 0; p.y < height; p.y++)
		{
			for (p.x = infoheader.width; p.x < width; p.x++)
			{
				s->setPixel(p, Foliage::Colors::Transparent);
			}
		}
	#endif
		
	return s;
}

Foliage::Surface* Foliage::BmpLoader::loadBmp(const std::string& filename) 
{
	SurfaceDictionary::iterator i = surfaces.find(filename);
	if (i == surfaces.end())
	{
		// asked surface is not loaded yet
		Foliage::Surface* s = performLoadBmp(filename);
		std::cout << "BmpLoader: " << filename << " loaded." << std::endl;
		surfaces[filename] = s;
		return s;
	}
	else
	{
		return i->second;
	}
}