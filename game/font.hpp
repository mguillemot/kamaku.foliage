#ifndef __FONT
#define __FONT

#include <map>
#include <string>
#include "foliage.hpp"

typedef std::map<const char, Foliage::Surface*> charactersMap;

class Font
{
public:
	Font();
	~Font();
	void drawString(const std::string s, const Foliage::Point p) const;
	Foliage::Size getCharacterSize() const { return _characterSize; }
	
private:
	charactersMap _characters;
	Foliage::Size _characterSize;
};

#endif //__FONT
