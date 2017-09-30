#pragma once

#include "rectangle.h"
#include "globals.h"
#include <string>

class Door : public Rectangle {
public:
	Door() :
	destination("invalid") {}
	Door(Rectangle r, std::string destination) :
		Rectangle(r.getLeft() * globals::SPRITE_SCALE, r.getTop() * globals::SPRITE_SCALE, r.getWidth() * globals::SPRITE_SCALE, r.getHeight() * globals::SPRITE_SCALE),
		destination(destination) {}
	inline Rectangle getRectangle() const { return this->getRect(); }
	inline std::string getDestination() const { return this->destination; };
private:
	std::string destination;
};