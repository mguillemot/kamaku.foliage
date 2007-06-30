#ifndef __FOLIAGE_HITBOX
#define __FOLIAGE_HITBOX

#include <vector>
#include "../basic_types.hpp"
#include "graphic_types.hpp"

namespace Foliage
{
	class Hitbox
	{
	public:
		void drawCollidableZoneAt(const Color color, const Point p) const
		{
			for (std::vector<Rect>::const_iterator i = _rects.begin(); i != _rects.end(); ++i)
			{
				Rect r = *i;
				r.shift(p);
				Screen::fillRect(r, color);
			}
		}

		static bool collisionTest(const Point myPos, const Hitbox *myHitbox, const Point hisPos, const Hitbox *hisHitbox)
		{
			for (std::vector<Rect>::const_iterator i = myHitbox->_rects.begin(); i != myHitbox->_rects.end(); ++i)
			{
				Rect r = *i;
				r.shift(myPos);
				for (std::vector<Rect>::const_iterator j = hisHitbox->_rects.begin(); j != hisHitbox->_rects.end(); ++j)
				{
					Rect s = *j;
					s.shift(hisPos);
					if (Rect::intersects(r, s))
					{
						return true;
					}
				}
			}
			return false;
		}

		void addRect(const Rect &hitbox)
		{
			_rects.push_back(hitbox);
		}

		const std::vector<Rect> &rects() const
		{
			return _rects;
		}

	protected:
		std::vector<Rect> _rects;
	};
}

#endif //__FOLIAGE_HITBOX
