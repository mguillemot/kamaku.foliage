#ifndef _FOLIAGE__BMP_LOADER
#define _FOLIAGE__BMP_LOADER

#include <map>
#include <string>
#include "surface.hpp"

namespace Foliage
{

	typedef std::map<const std::string, Surface*> SurfaceDictionary;

	class BmpLoader
	{
	public:
		static Surface* loadBmp(const std::string& filename);

	private:
		BmpLoader() {}

		static SurfaceDictionary surfaces;
	};

}

#endif //_FOLIAGE__BMP_LOADER
