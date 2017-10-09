#include <math.h>
#include <cmath>
#include "enemy.h"

// Base Enemy class

Enemy::Enemy() {};

Enemy::Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	facing(0.0),
	maxHealth(0),
	currentHealth(0)
{}

void Enemy::update(int elapsedTime, Player &player) {
	this->x += this->dx * elapsedTime;
	this->y += this->dy * elapsedTime;
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->x, this->y, this->facing);
}

void Enemy::changeHealth(int amount) {
	this->currentHealth += amount;
}

void Enemy::setInSight(bool enemyInSight) {
	this->inSight = enemyInSight;
}

bool Enemy::checkInSight() {
	return this->inSight;
}

void Enemy::setFacing(float x, float y) {
	this->facing = atan2(y - this->getCenterY(), x - this->getCenterX()) * 180.0000 / 3.1416;
}

void Enemy::move() {
	this->dx = cos(facing * 3.1416 / 180.0000) * this->speed;
	this->dy = sin(facing * 3.1416 / 180.0000) * this->speed;
}

void Enemy::stopMoving() {
	this->dx = 0.0f;
	this->dy = 0.0f;
}


// Alien Enemy class

Alien::Alien() {}

Alien::Alien(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "sprites/alien.png", 0, 0, 16, 16, spawnPoint, 140)
{
	this->setupAnimation();
	this->playAnimation("idle");
	this->speed = 0.4f;
	this->lastKnownPlayerLocation = Vector2(-1, -1);
	this->maxHealth = 3;
	this->currentHealth = 3;
}

void Alien::update(int elapsedTime, Player &player) {
	if (this->currentHealth > 0) {
		this->distanceToPlayer = sqrt(std::pow(abs(this->getCenterX() - player.getCenterX()), 2) + std::pow(abs(this->getCenterY() - player.getCenterY()), 2));
		if (this->distanceToPlayer < 500 && this->inSight) {
			// Player is within range and sight
			this->lastKnownPlayerLocation = Vector2(player.getCenterX(), player.getCenterY());
			this->setFacing(player.getCenterX(), player.getCenterY());
			if (this->distanceToPlayer > this->getBoundingBox().getWidth() / 2) {
				// Pursue player
				this->move();
			}
			else {
				// Player is within reach - attack
				this->touchPlayer(&player);
				this->stopMoving();
			}
			this->playAnimation("alert");
		}
		else if (this->lastKnownPlayerLocation != Vector2(-1, -1)) {
			// Player has been spotted but is no longer in view - move to last known location
			float distance2 = sqrt(std::pow(abs(this->getCenterX() - this->lastKnownPlayerLocation.x), 2) + std::pow(abs(this->getCenterY() - this->lastKnownPlayerLocation.y), 2));
			if (distance2 <= this->getBoundingBox().getWidth() / 2) {
				// last known player location reached
				this->lastKnownPlayerLocation = Vector2(-1, -1);
				this->stopMoving();
				this->playAnimation("idle");
			}
		}
	}
	else {
		// Alien is dead
		this->playAnimation("die");
		this->stopMoving();
	}
	
	Enemy::update(elapsedTime, player);
}

void Alien::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void Alien::touchPlayer(Player* player) {
	player->changeHealth(-1);
}

void Alien::animationDone(std::string currentAnimation) {
	if (currentAnimation != "die") {
		this->frameIndex = 0;
	}
}

void Alien::setupAnimation() {
	this->addAnimation(1, 0, 0, "idle", 16, 16, Vector2(0, 0));
	this->addAnimation(4, 0, 32, "alert", 16, 16, Vector2(0, 0));
	this->addAnimation(4, 0, 48, "die", 16, 16, Vector2(0, 0));
}