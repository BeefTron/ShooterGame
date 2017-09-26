#include "sprite.h"
#include "graphics.h"
#include "globals.h"

Sprite::Sprite() {}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) :
	x(posX),
	y(posY)
{
	this->sourceRect.x = sourceX;
	this->sourceRect.y = sourceY;
	this->sourceRect.w = width;
	this->sourceRect.h = height;

	this->spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (this->spriteSheet == NULL) {
		printf("\nError: Unable to load image\n");
	}
	this->boundingBox = Rectangle(this->x, this->y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}

Sprite::~Sprite() {}

void Sprite::draw(Graphics &graphics, int x, int y) {
	SDL_Rect destinationRectangle = { x, y, this->sourceRect.w * globals::SPRITE_SCALE, this->sourceRect.h * globals::SPRITE_SCALE };
	graphics.blitSurface(this->spriteSheet, &this->sourceRect, &destinationRectangle);
}

void Sprite::update() {
	this->updateBoundingBox(this->x, this->y, NULL, NULL);
}

Rectangle Sprite::getBoundingBox() const {
	return this->boundingBox;
}

sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	int amtRight = abs(this->getBoundingBox().getRight() - other.getLeft());
	int amtLeft = abs(other.getRight() - this->getBoundingBox().getLeft());
	int amtTop = abs(other.getBottom() - this->getBoundingBox().getTop());
	int amtBottom = abs(this->getBoundingBox().getBottom() - other.getTop());
	int sideAmounts[4] = { amtRight, amtLeft, amtTop, amtBottom };
	sides::Side sideTypes[4] = { sides::RIGHT, sides::LEFT, sides::TOP, sides::BOTTOM };

	int lowest = 0;
	for (int i = 0; i < 4; i++) {
		if (sideAmounts[i] < sideAmounts[lowest]) {
			lowest = i;
		}
	}
	return sideTypes[lowest];
}

void Sprite::updateBoundingBox(int x, int y, int width, int height) {
	if (x != NULL) {
		this->boundingBox.setX(x);
	}
	if (y != NULL) {
		this->boundingBox.setY(y);
	}
	if (width != NULL) {
		this->boundingBox.setWidth(width);
	}
	if (height != NULL) {
		this->boundingBox.setHeight(height);
	}
}