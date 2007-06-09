#ifndef _FOLIAGE__ROTATIVE_SURFACE
#define _FOLIAGE__ROTATIVE_SURFACE

#include "../basic_types.hpp"
#include "surface.hpp"

namespace Foliage
{
	class RotativeSurface
	{
	public:
		void setSurface(const Direction direction, Surface *surface)
		{
			_surfaces[direction] = surface;
		}

		Surface *surface(const Direction dir) const
		{
			return _surfaces[dir];
		}

		Size getSize()
		{
			return _surfaces[0]->getSize();
		}

	protected:
		Surface  *_surfaces[16];
	};
}

#endif //_FOLIAGE__ROTATIVE_SURFACE
