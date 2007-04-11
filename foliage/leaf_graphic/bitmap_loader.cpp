#include <iostream>
#include "bitmap_loader.hpp"

Foliage::StringMap Foliage::BitmapLoader::surfaces;

Foliage::Surface *Foliage::BitmapLoader::loadBitmap(const std::string &bitmap)
{
	StringMap::iterator i = surfaces.find(bitmap);
	if (i == surfaces.end())
	{
		// asked bitmap is not loaded yet
		Foliage::Surface *s = Foliage::Surface::readBMP(bitmap);
		std::cout << "BitmapLoader: " << bitmap << " loaded." << std::endl;
		surfaces[bitmap] = s;
		return s;
	}
	else
	{
		return (*i).second;
	}
}
