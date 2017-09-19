#pragma once

#include "animatedsprite.h"
#include "globals.h"

class Graphics;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(int elapsedTime);

	void move(int vertical, int horizontal, float speedMult = 1);
	void stopMoving();
	void setFacing(int mouseX, int mouseY);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimation();

	float getX() const;
	float getY() const;

	// Handles collisions with all tiles the player is colliding with
	void handleTileCollisions(std::vector<Rectangle> &others);
private:
	float dx, dy;

	//Direction facing;
	float facing;
};