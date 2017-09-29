#pragma once

#include "sprite.h"
#include "player.h"
#include <vector>

class Graphics;

class HUD {
public:
	HUD();
	HUD(Graphics &graphics, Player player);
	void update(int elapsedTime);
	void draw(Graphics &graphics);
private:
	Player player;
	Sprite healthBarSprite;
	Sprite currentHealthSprite;
	std::vector<Sprite> livesSprites;
};