#ifndef __HITBOX
#define __HITBOX

#include <vector>
#include "foliage.hpp"

class Hitbox
{
public:
	void drawCollidableZoneAt(const Foliage::Color color, const Foliage::Point p) const
	{
		for (std::vector<Foliage::Rect>::const_iterator i = _rects.begin(); i != _rects.end(); ++i)
		{
			Foliage::Rect r = *i;
			r.shift(p);
			Foliage::Screen::fillRect(r, color);
		}
	}

	static bool collisionTest(const Foliage::Point myPos, const Hitbox *myHitbox, const Foliage::Point hisPos, const Hitbox *hisHitbox)
	{
		for (std::vector<Foliage::Rect>::const_iterator i = myHitbox->_rects.begin(); i != myHitbox->_rects.end(); ++i)
		{
			Foliage::Rect r = *i;
			r.shift(myPos);
			for (std::vector<Foliage::Rect>::const_iterator j = hisHitbox->_rects.begin(); j != hisHitbox->_rects.end(); ++j)
			{
				Foliage::Rect s = *j;
				s.shift(hisPos);
				if (Foliage::Rect::intersects(r, s))
				{
					return true;
				}
			}
		}
		return false;
	}

	void addRect(const Foliage::Rect &hitbox)
	{
		_rects.push_back(hitbox);
	}

	const std::vector<Foliage::Rect>& rects() const
	{
		return _rects;
	}

protected:
	std::vector<Foliage::Rect> _rects;
};

#endif //__HITBOX
