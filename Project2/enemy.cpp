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
	this->speed = 0.1f;
	this->lastKnownPlayerLocation = Vector2(-1, -1);
	this->maxHealth = 3;
	this->currentHealth = 3;
}

void Alien::update(int elapsedTime, Player &player) {
	float distance = sqrt(std::pow(abs(this->getCenterX() - player.getCenterX()), 2) + std::pow(abs(this->getCenterY() - player.getCenterY()), 2));
	if (distance < 500 && this->inSight) {
		// Player is within range and sight - chase
		this->lastKnownPlayerLocation = Vector2(player.getCenterX(), player.getCenterY());
		this->setFacing(player.getCenterX(), player.getCenterY());
		if (distance > 1) {
			this->move();
		}
		else {
			this->stopMoving();
		}
		this->playAnimation("alert");
	}
	else if (this->lastKnownPlayerLocation != Vector2(-1, -1)) {
		// Player has been spotted but is no longer in view - moving to last known location
		float distance2 = sqrt(std::pow(abs(this->getCenterX() - this->lastKnownPlayerLocation.x), 2) + std::pow(abs(this->getCenterY() - this->lastKnownPlayerLocation.y), 2));
		if (distance2 <= 1) {
			// last known player location reached - player lost
			this->lastKnownPlayerLocation = Vector2(-1, -1);
			this->stopMoving();
			this->playAnimation("idle");
		}
	}
	Enemy::update(elapsedTime, player);
}

void Alien::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void Alien::animationDone(std::string currentAnimation) {

}

void Alien::setupAnimation() {
	this->addAnimation(1, 0, 0, "idle", 16, 16, Vector2(0, 0));
	this->addAnimation(4, 0, 32, "alert", 16, 16, Vector2(0, 0));
}