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
	health(100),
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
	this->addAnimation(2, 0, 8, "standAndShoot", 15, 8, Vector2(0, 0));
	this->addAnimation(4, 0, 16, "moveAndShoot", 15, 8, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {}

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
	float rads = atan2(mouseY - this->getYCenter(), mouseX - this->getXCenter());
	this->facing = rads * 180.0000 / 3.1416;
}

void Player::shoot() {
	this->shooting = true;
}

void Player::holdFire() {
	this->shooting = false;
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

void Player::handleDoorCollision(Door &door, Level &level, Graphics &graphics) {
	std::string from = level.getMapName();
	level = Level(door.getDestination(), graphics);
	this->x = level.getPlayerSpawn(from).x;
	this->y = level.getPlayerSpawn(from).y;
}

void Player::update(int elapsedTime) {
	this->x += this->dx * elapsedTime;
	this->y += this->dy * elapsedTime;
	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->x, this->y, this->facing, &player_constants::PLAYER_CENTER);
}