#include "animatedtile.h"
#include "graphics.h"

AnimatedTile::AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 position) :
	Tile(tileset, size, tilesetPositions.at(0), position),
	tilesetPositions(tilesetPositions),
	duration(duration),
	tileToDraw(0)
{}

void AnimatedTile::update(int elapsedTime) {
	if (this->amountOfTime <= 0) {
		if (this->tileToDraw == this->tilesetPositions.size() - 1) {
			this->tileToDraw = 0;
		}
		else {
			this->tileToDraw++;
		}
		this->amountOfTime = this->duration;
	}
	else {
		this->amountOfTime -= elapsedTime;
	}
	Tile::update(elapsedTime);
}

void AnimatedTile::draw(Graphics &graphics) {
	SDL_Rect destRect = { this->mapPos.x, this->mapPos.y, this->size.x * globals::SPRITE_SCALE, this->size.y * globals::SPRITE_SCALE };
	SDL_Rect sourceRect = { this->tilesetPositions.at(this->tileToDraw).x, this->tilesetPositions.at(tileToDraw).y, this->size.x, this->size.y };
	graphics.blitSurface(this->tileset, &sourceRect, &destRect);
}