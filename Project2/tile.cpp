#include "tile.h"
#include "graphics.h"
#include "SDL.h"

Tile::Tile() {}

Tile::Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPos, Vector2 mapPos) :
	tileset(tileset),
	size(size),
	tilesetPos(tilesetPos),
	mapPos(Vector2(mapPos.x * globals::SPRITE_SCALE, mapPos.y * globals::SPRITE_SCALE))
{}

void Tile::update(int elapsedTime) {}

void Tile::draw(Graphics &graphics) {
	SDL_Rect destRect = { this->mapPos.x, this->mapPos.y, this->size.x * globals::SPRITE_SCALE, this->size.y * globals::SPRITE_SCALE };
	SDL_Rect sourceRect = { this->tilesetPos.x, this->tilesetPos.y, this->size.x, this->size.y };

	graphics.blitSurface(this->tileset, &sourceRect, &destRect);
}