#ifndef _FOLIAGE__SURFACE
#define _FOLIAGE__SURFACE

#include <string>
#include "../basic_types.hpp"
#include "graphic_types.hpp"

namespace Foliage
{

	class Surface
	{
	public:
		// Constructor & destructor
		~Surface();
		
		// Instance members
		Color  getPixel(const Point p) const;
		void   setPixel(const Point p, const Color color);
		void   drawAt(const Point p) const;
		Size   getSize() const { return _size; }
		const  std::string &getName() const { return _name; }
		Color *getPixels() const { return _pixels; }
		
		#ifdef __PPC__
			void   setInstancized(const Sint32 instancized) { _instancized = instancized; }
			Sint32 getInstancized() const { return _instancized; }
			Surface *createNewShiftedSurface(const Sint32 shift) const;
		#else
			SDL_Surface *getSDLSurface() const { return _SDLSurface; }
		#endif

		// Static members
		static Surface *readBMP(const std::string &filename);

	private:
		Surface(const Size s, Color *pixels, const std::string &name);

		Size _size;
		std::string _name;
		Color *_pixels;
		#ifdef __PPC__
			Sint32 _instancized;
		#else
			SDL_Surface *_SDLSurface;
		#endif
	};
	
}

#endif //_FOLIAGE__SURFACE
