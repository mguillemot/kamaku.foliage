#include <iostream>
#include "screen.hpp"
#include "sprite.hpp"
#include "colors.hpp"
#include "bitmap_loader.hpp"

Foliage::Sprite::Sprite(const std::string &filename) 
	: _x(), _y(), _delayBetweenFrames(1), _delayCount(1)
{
	_hitbox.w = 0;
	_hitbox.h = 0;
	_frames.push_back(Foliage::BitmapLoader::loadBitmap(filename));
	_currentFrame = _frames.begin();
}

Foliage::Sprite::Sprite(Foliage::Surface *surface)
	: _x(), _y(), _delayBetweenFrames(1), _delayCount(1)
{
	_hitbox.w = 0;
	_hitbox.h = 0;
	_frames.push_back(surface);
	_currentFrame = _frames.begin();
}

void Foliage::Sprite::addFrame(const std::string &filename)
{
	_frames.push_back(Foliage::BitmapLoader::loadBitmap(filename));
}

void Foliage::Sprite::addFrame(Foliage::Surface *surface)
{
	_frames.push_back(surface);
}

void Foliage::Sprite::changeFrame(Foliage::Surface *surface)
{
	_frames.clear();
	_frames.push_back(surface);
	_currentFrame = _frames.begin();
}

Foliage::Sprite::~Sprite()
{
	// TODO: delete unused surfaces
	// (Warning: sufaces can be shared between sprites!)
}

Foliage::Rect Foliage::Sprite::getScreenHitbox() const
{
	Rect screenHitbox = _hitbox;
	screenHitbox.x += Sint16(_x);
	screenHitbox.y += Sint16(_y);
	return screenHitbox;	
}

void Foliage::Sprite::setHitbox(const Foliage::Rect &hitbox)
{
	_hitbox = hitbox;
}

void Foliage::Sprite::draw() const
{
	//if (!outOfScreen())
	{
		(*_currentFrame)->drawAt(getPosition());
	}
}

void Foliage::Sprite::drawHitbox(const Foliage::Color color) const
{
	Screen::fillRect(getScreenHitbox(), color);
}

void Foliage::Sprite::setPosition(const Foliage::Point p)
{
	_x = Foliage::Fixed(p.x);
	_y = Foliage::Fixed(p.y);
}

Foliage::Point Foliage::Sprite::getPosition() const
{
	return Foliage::Point(Sint16(_x), Sint16(_y));
}

Foliage::Point Foliage::Sprite::getCenter() const
{
	const Foliage::Size s = getSize();
	Foliage::Point pos = getPosition();
	pos.x += s.w >> 1;
	pos.y += s.h >> 1;
	return pos;
}

void Foliage::Sprite::setSpeed(const Foliage::Speed s)
{
	_s = s;
}

Foliage::Speed Foliage::Sprite::getSpeed() const
{
	return _s;
}

void Foliage::Sprite::move()
{
	_x += _s.x;
	_y += _s.y;
	_delayCount--;
	if (_delayCount == 0)
	{
		++_currentFrame;
		_delayCount = _delayBetweenFrames;
		if (_currentFrame == _frames.end())
		{
			_currentFrame = _frames.begin();
		}
	}
}

void Foliage::Sprite::setDelayBetweenFrames(const Sint32 delay)
{
	_delayBetweenFrames = delay;
	_delayCount = delay;
}

bool Foliage::Sprite::outOfScreen() const
{
	const Foliage::Point pos = getPosition();
	const Foliage::Size size = getSize();
	return (pos.y >= Foliage::Screen::Height || (pos.y + size.h) <= 0
		|| pos.x >= Foliage::Screen::Width || (pos.x + size.w) <= 0);
}

Foliage::Size Foliage::Sprite::getSize() const
{
	return (*_currentFrame)->getSize();
}

void Foliage::Sprite::inspect()
{
	const Foliage::Point pos = getPosition();
	std::cout << "x=" << _x << " y=" << _y << " pos=(" << pos.x << "," << pos.y << ")" << std::endl;
	std::cout << "speed=(" << _s.x << "," << _s.y << std::endl;
}
