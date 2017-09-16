#pragma once

#include <string>
#include "globals.h"

class Graphics;
struct SDL_Texture;

class Level {
public:
	Level();
	Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics);
	~Level();
	void update(int elapsedTime);
	void draw(Graphics &graphics);
private:
	std::string mapName;
	Vector2 spawnPoint;
	Vector2 size;
	SDL_Texture* backgroundTexture;

	void loadMap(std::string mapName, Graphics &graphics);
};