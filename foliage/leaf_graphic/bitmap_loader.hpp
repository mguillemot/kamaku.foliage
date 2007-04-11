#ifndef _FOLIAGE__BITMAP_LOADER
#define _FOLIAGE__BITMAP_LOADER

#include <string>
#include <map>
#include "surface.hpp"

namespace Foliage
{

	typedef std::map<const std::string, Surface*> StringMap;

	class BitmapLoader
	{
	public:
		static Surface *loadBitmap(const std::string &bitmap);
	
	private:
		static StringMap surfaces;
	};	
	
}

#endif //_FOLIAGE__BITMAP_LOADER
