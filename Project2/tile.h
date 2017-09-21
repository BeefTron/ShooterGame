#pragma once

#include "globals.h"

struct SDL_Texture;
class Graphics;

class Tile {
public:
	Tile();
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPos, Vector2 mapPos);
	void update(int elapsedTime);
	void draw(Graphics &graphics);
protected:
	SDL_Texture* tileset;
	Vector2 size;
	Vector2 tilesetPos;
	Vector2 mapPos;
};