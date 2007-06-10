#ifndef _FOLIAGE__SURFACE
#define _FOLIAGE__SURFACE

#include <string>
#include <list>
#include <vector>
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
		Color    getPixel(const Point p) const;
		void     setPixel(const Point p, const Color color);
		void     fill(const Color color);
		void     drawLine(const Point from, const Point to, const Color color);
		void     drawRect(const Rect &r, const Color color);
		void     fillRect(const Rect &r, const Color color);
		void     drawAt(const Point p) const;
		Size     getSize() const { return _size; }
		const    std::string &getName() const { return _name; }
		Rect     getBoundingRect() const;

		Surface *createNewShiftedSurface(const Sint32 shift) const;
		Surface *createNewRotatedSurface(const Sint32 angle) const;
		
		#ifdef __PPC__
			Color *getPixels() const { return _pixels; }
			void   setInstancized(const Sint32 instancized) { _instancized = instancized; }
			Sint32 getInstancized() const { return _instancized; }
		#else
			void lock() const;
			void unlock() const;
			SDL_Surface *getSDLSurface() const { return _SDLSurface; }
		#endif

		// Static members
		static Surface *readBMP(const std::string &filename);
		static Surface *createEmptySurface(const Size s, const std::string &name);

	private:
		Surface(const Size s, Color *pixels, const std::string &name);

		Size _size;
		std::string _name;
		#ifdef __PPC__
			Color *_pixels;
			Sint32 _instancized;
		#else
			SDL_Surface *_SDLSurface;
			mutable Sint32 _locks;
		#endif
	};
	
	typedef std::list<Surface *> ListSurface;
	typedef std::list<Surface *> VectorSurface;

}

#endif //_FOLIAGE__SURFACE
