#ifndef _FOLIAGE__BITMAP_LOADER
#define _FOLIAGE__BITMAP_LOADER

#include <string>
#include <map>
#include "surface.hpp"

namespace Foliage
{
	typedef std::map<const std::string, Surface *> SurfaceDictionary;

	class BitmapLoader
	{
	public:
		static Surface *loadBitmap(const std::string &bitmap);
	
	private:
		static SurfaceDictionary surfaces;
	};	
	
}

#endif //_FOLIAGE__BITMAP_LOADER
