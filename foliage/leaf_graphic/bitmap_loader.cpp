#include <iostream>
#include "bitmap_loader.hpp"

Foliage::SurfaceDictionary Foliage::BitmapLoader::surfaces;

Foliage::Surface *Foliage::BitmapLoader::loadBitmap(const std::string &bitmap)
{
	SurfaceDictionary::iterator i = surfaces.find(bitmap.c_str());
	Surface *result = surfaces[bitmap];
	if (result == NULL)
	{
		// asked bitmap is not loaded yet
		Foliage::Surface *s = Foliage::Surface::readBMP(bitmap);
		std::cout << "BitmapLoader: " << bitmap << " loaded." << std::endl;
		surfaces[bitmap] = s;
		return s;
	}
	else
	{
		return result;
	}
}
