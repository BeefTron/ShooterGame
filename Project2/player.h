#pragma once

#include "animatedsprite.h"
#include "globals.h"

class Graphics;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, int x, int y);
	void draw(Graphics &graphics);
	void update(int elapsedTime);

	void move(int vertical, int horizontal, float speedMult = 1);
	void stopMoving();
	void setFacing(int mouseX, int mouseY);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimation();
private:
	float dx, dy;

	//Direction facing;
	float facing = 45.0f;
};