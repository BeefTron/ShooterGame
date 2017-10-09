#pragma once

#include "animatedsprite.h"
#include "level.h"
#include "globals.h"
#include "enemy.h"

class Graphics;

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const SDL_Point PLAYER_CENTER = { 4 * globals::SPRITE_SCALE, 4 * globals::SPRITE_SCALE };
	const int MAXHEALTH = 50;
}

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(int elapsedTime);

	void move(int vertical, int horizontal, float speedMult = 1);
	void stopMoving();
	void setFacing(int mouseX, int mouseY);
	void setShooting(bool shooting);
	inline bool isShooting() { return this->shooting; }
	void checkBulletCollision(Enemy* enemy);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimation();

	inline float getCenterX() const { return this->x + player_constants::PLAYER_CENTER.x; }
	inline float getCenterY() const { return this->y + player_constants::PLAYER_CENTER.y; }

	inline int getHealth() { return this->health; }
	void changeHealth(int amount);
	inline int getLives() { return this->lives; }
	void changeLives(int amount);

	// Handles collisions with all tiles the player is colliding with
	void handleTileCollisions(std::vector<Rectangle> &others);
	// Not currently used. Enemy damage dealt by enemy update function.
	void handleEnemyCollisions(std::vector<Enemy*> &enemies);
	void handleDoorCollision(Door &door, Level &level, Graphics &graphics);
private:
	float dx, dy;
	bool shooting = false;
	float facing;
	int health;
	int lives;
};