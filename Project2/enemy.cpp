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
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->x, this->y, this->facing);
}

void Enemy::setFacing(float x, float y) {
	this->facing = atan2(y - this->getYCenter(), x - this->getXCenter()) * 180.0000 / 3.1416;
}


// TestEnemy class

TestEnemy::TestEnemy() {}

TestEnemy::TestEnemy(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "sprites/arrow.png", 0, 0, 16, 16, spawnPoint, 140)
{
	this->setupAnimation();
	this->playAnimation("idle");
}

void TestEnemy::update(int elapsedTime, Player &player) {
	float distance = sqrt(std::pow(abs(this->getXCenter() - player.getXCenter()), 2) + std::pow(abs(this->getYCenter() - player.getYCenter()), 2));
	if (distance < 200) {
		this->setFacing(player.getXCenter(), player.getYCenter());
		this->playAnimation("idleRed");
	}
	else {
		this->playAnimation("idle");
	}
	Enemy::update(elapsedTime, player);
}

void TestEnemy::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void TestEnemy::animationDone(std::string currentAnimation) {

}

void TestEnemy::setupAnimation() {
	this->addAnimation(1, 0, 0, "idle", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 16, "idleRed", 16, 16, Vector2(0, 0));
}