#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "SDL.h"

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) :
	mapName(mapName),
	spawnPoint(spawnPoint),
	size(Vector2()) 
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {}

void Level::loadMap(std::string mapName, Graphics &graphics) {
	this->backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("backgrounds/tile.png"));
	this->size = Vector2(globals::SCREEN_WIDTH * 2, globals::SCREEN_HEIGHT * 2);
}

void Level::update(int elapsedTime) {}

void Level::draw(Graphics &graphics) {
	SDL_Rect sourceRect = { 0, 0, 64, 64 };
	SDL_Rect destRect;
	for (int x = 0; x < this->size.x / 64; x++) {
		for (int y = 0; y < this->size.y / 64; y++) {
			destRect.x = x * 64 * globals::SPRITE_SCALE;
			destRect.y = y * 64 * globals::SPRITE_SCALE;
			destRect.w = 64 * globals::SPRITE_SCALE;
			destRect.h = 64 * globals::SPRITE_SCALE;
			graphics.blitSurface(this->backgroundTexture, &sourceRect, &destRect);
		}
	}
}