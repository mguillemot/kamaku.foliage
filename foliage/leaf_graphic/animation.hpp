#ifndef _FOLIAGE__ANIMATION
#define _FOLIAGE__ANIMATION

#include <vector>
#include "../basic_types.hpp"
#include "surface.hpp"
#include "rotative_surface.hpp"

namespace Foliage
{
	template<typename SurfaceType>
	class Animation
	{
	public:
		Animation(const Sint32 nbFrames)
		{
			_frames.reserve(nbFrames);
			_currentFrame = _frames.begin();
		}

		void add(SurfaceType *frame)
		{
			_frames.push_back(frame);
			_currentFrame = _frames.begin();
		}

		SurfaceType *current() const
		{
			return *_currentFrame;
		}

		void next()
		{
			++_currentFrame;
			if (_currentFrame == _frames.end())
			{
				_currentFrame = _frames.begin();
			}
		}

	protected:
		typename std::vector<SurfaceType *>                 _frames;
		typename std::vector<SurfaceType *>::const_iterator _currentFrame;
	};

	typedef Animation<Surface>         SimpleAnimation;
	typedef Animation<RotativeSurface> RotativeAnimation;
}

#endif //_FOLIAGE__ANIMATION
