#ifndef _FOLIAGE__BITMAP_LOADER
#define _FOLIAGE__BITMAP_LOADER

#include <string>
#include <hash_map>
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

	typedef stdext::hash_map<const std::string, Surface*> SurfaceDictionary;

	class BitmapLoader
	{
	public:
		static Surface *loadBitmap(const std::string &bitmap);
	
	private:
		static SurfaceDictionary surfaces;
	};	
	
}

#endif //_FOLIAGE__BITMAP_LOADER
