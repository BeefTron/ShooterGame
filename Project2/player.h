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
	void shoot();
	void holdFire();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimation();

	float getX() const;
	float getY() const;

	inline int getHealth() { return this->health; }
	inline int getLives() { return this->lives; }

	// Handles collisions with all tiles the player is colliding with
	void handleTileCollisions(std::vector<Rectangle> &others);
private:
	float dx, dy;
	bool shooting = false;
	float facing;
	int health;
	int lives;
};