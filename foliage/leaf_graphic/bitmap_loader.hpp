#ifndef _FOLIAGE__BITMAP_LOADER
#define _FOLIAGE__BITMAP_LOADER

#include <string>
#include <map>
#include "surface.hpp"

namespace Foliage
{

	struct ltstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return strcmp(s1, s2) < 0;
		}
	};

	typedef std::map<const char*, Surface*, ltstr> SurfaceMap;

	class BitmapLoader
	{
	public:
		static Surface *loadBitmap(const std::string &bitmap);
	
	private:
		static SurfaceMap surfaces;
	};	
	
}

#endif //_FOLIAGE__BITMAP_LOADER
