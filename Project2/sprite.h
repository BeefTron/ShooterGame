#pragma once

#include "SDL.h"
#include <string>
#include "rectangle.h"
#include "globals.h"

class Graphics;

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY);
	virtual ~Sprite();
	virtual void update();
	void draw(Graphics &graphics, int x, int y);
	Rectangle getBoundingBox() const;
	// Determines which side the collision happened on
	sides::Side getCollisionSide(Rectangle &other) const;
protected:
	SDL_Rect sourceRect;
	SDL_Texture* spriteSheet;
	float x, y;
	Rectangle boundingBox;
private:
};