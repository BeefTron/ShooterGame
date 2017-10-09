#pragma once

#include "animatedsprite.h"
#include "globals.h"
#include "player.h"
#include <string>

class Graphics;

class Enemy : public AnimatedSprite {
public:
	Enemy();
	Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);
	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);
	virtual void touchPlayer(Player* player) = 0;

	inline int getMaxHealth() const { return this->maxHealth; }
	inline int getHealth() const { return this->currentHealth; }
	inline float getDistanceToPlayer() const { return this->distanceToPlayer; }
	void changeHealth(int amount);
	void setInSight(bool enemyInSight);
	bool checkInSight();
protected:
	float facing; // -180 to 180 degrees
	int maxHealth;
	int currentHealth;
	void setFacing(float x, float y);
	float speed = 0;
	float dx, dy;
	// Enemy will move in whatever direction they are facing
	void move();
	void stopMoving();
	// Whether the player and the Enemy are within sight of each other
	bool inSight;
	float distanceToPlayer;
};

class Alien : public Enemy {
public:
	Alien();
	Alien(Graphics &graphics, Vector2 spawnPoint);
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void touchPlayer(Player* player);
	void animationDone(std::string currentAnimation);
	void setupAnimation();
private:
	Vector2 lastKnownPlayerLocation;

};