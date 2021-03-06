#pragma once

#include <vector>
#include "tile.h"
#include "globals.h"

class AnimatedTile : public Tile {
public:
	AnimatedTile(std::vector<Vector2> tilesetPositions, std::vector<int> durations, SDL_Texture* tileset, Vector2 size, Vector2 position);
	void update(int elapsedTime);
	void draw(Graphics &graphics);
protected:
	int amountOfTime = 0;
	bool notDone = false;
private:
	std::vector<Vector2> tilesetPositions;
	int tileToDraw;
	std::vector<int> durations;
};

struct AnimatedTileInfo {
	int TilesetsFirstGid;
	int StartTileId;
	std::vector<int> TileIds;
	std::vector<int> Durations;
};