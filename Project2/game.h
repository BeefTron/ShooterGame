#pragma once

#include "player.h"
#include "level.h"
#include "hud.h"
#include "graphics.h"

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics &graphics);
	void update(int elapsedTime);

	Player player;
	Level level;
	HUD hud;
	Graphics graphics;
};
