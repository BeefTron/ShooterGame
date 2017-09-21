#pragma once

#include <string>
#include <vector>
#include "globals.h"
#include "tile.h"
#include "rectangle.h"

class Graphics;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics);
	~Level();
	void update(int elapsedTime);
	void draw(Graphics &graphics);
	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
	Vector2 getPlayerSpawn() const;
private:
	std::string mapName;
	Vector2 spawnPoint;
	Vector2 size;
	Vector2 tileSize;
	SDL_Texture* backgroundTexture;
	std::vector<Tile> tileList;
	std::vector<Tileset> tilesets;
	std::vector<Rectangle> collisionRects;

	void loadMap(std::string mapName, Graphics &graphics);
};

//Tileset struct
struct Tileset {
	SDL_Texture* Texture;
	int FirstGid;
	int LastGid;
	Tileset() {
		this->FirstGid = -1;
		this->LastGid = -1;
	}
	Tileset(SDL_Texture* texture, int firstGid, int lastGid) {
		this->Texture = texture;
		this->FirstGid = firstGid;
		this->LastGid = lastGid;
	}
};