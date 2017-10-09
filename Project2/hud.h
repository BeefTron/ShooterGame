#pragma once

#include "sprite.h"
#include "player.h"
#include <vector>

class Graphics;

class HUD {
public:
	HUD();
	HUD(Graphics &graphics);
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
private:
	Sprite healthBarSprite;
	Sprite currentHealthSprite;
	std::vector<Sprite> livesSprites;
};