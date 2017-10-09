#include "player.h"
#include "graphics.h"
#include "globals.h"
#include <math.h>

Player::Player() {}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "sprites/PlayerCompressed.png", 0, 0, 15, 8, spawnPoint.x, spawnPoint.y, 100),
	dx(0),
	dy(0),
	facing(0.0f),
	health(player_constants::MAXHEALTH),
	lives(3)
{
	graphics.loadImage("sprites/PlayerCompressed.png");
	this->setupAnimation();
	this->playAnimation("move");
	this->updateBoundingBox(NULL, NULL, 9 * globals::SPRITE_SCALE, NULL);
}

void Player::setupAnimation() {
	this->addAnimation(1, 0, 0, "idle", 15, 8, Vector2(0, 0));
	this->addAnimation(4, 0, 0, "move", 15, 8, Vector2(0, 0));
	this->addAnimation(4, 0, 8, "standAndShoot", 15, 8, Vector2(0, 0));
	this->addAnimation(4, 0, 16, "moveAndShoot", 15, 8, Vector2(0, 0));
	this->addAnimation(6, 0, 32, "die", 15, 8, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {
	if (currentAnimation != "die") {
		this->frameIndex = 0;
	}
}

void Player::move(int horizontal, int vertical, float speedMult) {
	this->dx = player_constants::WALK_SPEED * horizontal * speedMult;
	this->dy = player_constants::WALK_SPEED * vertical * speedMult;
	if (this->shooting) {
		this->playAnimation("moveAndShoot");
	}
	else {
		this->playAnimation("move");
	}
}

void Player::stopMoving() {
	this->dx = 0.0f;
	this->dy = 0.0f;
	if (this->shooting) {
		this->playAnimation("standAndShoot");
	}
	else {
		this->playAnimation("idle");
	}
}

void Player::setFacing(int mouseX, int mouseY) {
	float rads = atan2(mouseY - this->getCenterY(), mouseX - this->getCenterX());
	this->facing = rads * 180.0000 / 3.1416;
}

void Player::setShooting(bool shooting) {
	this->shooting = shooting;
}

void Player::checkBulletCollision(Enemy* enemy) {
	float rads = atan2(enemy->getCenterY() - this->getCenterY(), enemy->getCenterX() - this->getCenterX());
	float enemyAngle = rads * 180.0000 / 3.1416;
	int enemyWidth = enemy->getBoundingBox().getWidth() / 2;
	float marginOfError = atan2(enemyWidth, enemy->getDistanceToPlayer()) * 180.0000 / 3.1416;
	float upper = enemyAngle + marginOfError;
	if (upper > 180) {
		upper -= 360;
	}
	float lower = enemyAngle - marginOfError;
	if (lower < -180) {
		lower += 360;
	}
	if (lower > upper) {
		// The hit range encompasses the angles 180 and -180 degrees. This requires different logic
		if (this->facing >= lower || this->facing <= upper) {
			enemy->changeHealth(-1);
		}
	}
	else if (this->facing >= lower && this->facing <= upper) {
		enemy->changeHealth(-1);
	}
}

void Player::changeHealth(int amount) {
	this->health += amount;
}

void Player::changeLives(int amount) {
	this->lives += amount;
}

void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->y = others.at(i).getBottom() + 1;
				break;
			case sides::BOTTOM:
				this->y = others.at(i).getTop() - this->boundingBox.getHeight() - 1;
				break;
			case sides::LEFT:
				this->x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->x = others.at(i).getLeft() - this->boundingBox.getWidth() - 1;
				break;
			}
		}
	}
}

void Player::handleEnemyCollisions(std::vector<Enemy*> &enemies) {
	for (int i = 0; i < enemies.size(); i++) {
		enemies.at(i)->touchPlayer(this);
	}
}

void Player::handleDoorCollision(Door &door, Level &level, Graphics &graphics) {
	std::string from = level.getMapName();
	level = Level(door.getDestination(), graphics);
	this->x = level.getPlayerSpawn(from).x;
	this->y = level.getPlayerSpawn(from).y;
}

void Player::update(int elapsedTime) {
	if (this->health > 0) {
		this->x += this->dx * elapsedTime;
		this->y += this->dy * elapsedTime;
	}
	else {
		this->playAnimation("die", true);
	}
	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->x, this->y, this->facing, &player_constants::PLAYER_CENTER);
}