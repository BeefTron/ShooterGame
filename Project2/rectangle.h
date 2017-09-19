#pragma once

#include "globals.h"

class Rectangle {
public:
	Rectangle() {}
	Rectangle(int x, int y, int width, int height) :
		x(x),
		y(y),
		width(width),
		height(height)
	{}
	int getCenterX() const { return this->x + this->width / 2; }
	int getCenterY() const { return this->y + this->height / 2; }
	int getLeft() const { return this->x; }
	int getRight() const { return this->x + this->width; }
	// Note that position is measured from top left. This means bottom >= top
	int getTop() const { return this->y; }
	int getBottom() const { return this->y + this->height; }

	int getWidth() const { return this->width; }
	int getHeight() const { return this->height; }

	int getSide(sides::Side side) const {
		return
			side == sides::LEFT ? this->getLeft() :
			side == sides::RIGHT ? this->getRight() :
			side == sides::TOP ? this->getTop() :
			side == sides::BOTTOM ? this->getBottom() :
			sides::NONE;
	}

	bool collidesWith(const Rectangle &other) const {
		return
			this->getRight() >= other.getLeft() &&
			this->getLeft() <= other.getRight() &&
			this->getTop() <= other.getBottom() &&
			this->getBottom() >= other.getTop();
	}

	bool isValidRectangle() const {
		return (this->x >= 0 && this->y >= 0 && this->width >= 0 && this->height >= 0);
	}

private:
	int x, y, width, height;
};