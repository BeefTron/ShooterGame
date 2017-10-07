#pragma once

#include <string>
#include <vector>
#include <map>
#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "animatedtile.h"
#include "door.h"

class Graphics;
class Enemy;
class Player;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	Level(std::string mapName, Graphics &graphics);
	~Level();
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	Door checkDoorCollisions(const Rectangle &playerBox);
	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
	bool checkPointCollisions(int x, int y) const;
	bool BresenhamSight(Vector2 p1, Vector2 p2) const;
	Vector2 getPlayerSpawn(std::string from = "");
	inline std::string getMapName() { return this->mapName; }
private:
	std::string mapName;
	std::map<std::string, Vector2> spawnPointMap;
	Vector2 size;
	Vector2 tileSize;
	SDL_Texture* backgroundTexture;
	std::vector<Tile> tileList;
	std::vector<Tileset> tilesets;
	std::vector<Rectangle> collisionRects;

	std::vector<AnimatedTile> animatedTileList;
	std::vector<AnimatedTileInfo> animatedTileInfos;

	std::vector<Door> doorList;

	std::vector<Enemy*> enemies;

	void loadMap(std::string mapName, Graphics &graphics);

	Vector2 getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);
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