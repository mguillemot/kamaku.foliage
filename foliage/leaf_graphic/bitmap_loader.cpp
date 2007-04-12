#include <iostream>
#include "bitmap_loader.hpp"

Foliage::SurfaceMap Foliage::BitmapLoader::surfaces;

Foliage::Surface *Foliage::BitmapLoader::loadBitmap(const std::string &bitmap)
{
	SurfaceMap::iterator i = surfaces.find(bitmap.c_str());
	if (i == surfaces.end())
	{
		// asked bitmap is not loaded yet
		Foliage::Surface *s = Foliage::Surface::readBMP(bitmap);
		std::cout << "BitmapLoader: " << bitmap << " loaded." << std::endl;
		surfaces[bitmap.c_str()] = s;
		return s;
	}
	else
	{
		return (*i).second;
	}
}
