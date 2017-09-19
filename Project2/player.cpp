#include "player.h"
#include "graphics.h"
#include "globals.h"
#include <math.h>

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const SDL_Point PLAYER_CENTER = { 5 * globals::SPRITE_SCALE, 8 * globals::SPRITE_SCALE };
}

Player::Player() {}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "sprites/Player.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	dx(0),
	dy(0),
	facing(0.0f)
{
	graphics.loadImage("sprites/Player.png");
	this->setupAnimation();
	this->playAnimation("move");
}

void Player::setupAnimation() {
	this->addAnimation(1, 0, 0, "idle", 16, 16, Vector2(0, 0));
	this->addAnimation(4, 0, 0, "move", 16, 16, Vector2(0,0));
	this->addAnimation(2, 0, 15, "shoot", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {}

float Player::getX() const {
	return this->x;
}

float Player::getY() const {
	return this->y;
}

void Player::move(int horizontal, int vertical, float speedMult) {
	this->dx = player_constants::WALK_SPEED * horizontal * speedMult;
	this->dy = player_constants::WALK_SPEED * vertical * speedMult;
	this->playAnimation("move");
}

void Player::stopMoving() {
	this->dx = 0.0f;
	this->dy = 0.0f;
	this->playAnimation("idle");
}

void Player::setFacing(int mouseX, int mouseY) {
	float playerX = this->x + (player_constants::PLAYER_CENTER.x);
	float playerY = this->y + (player_constants::PLAYER_CENTER.y);
	float rads = atan2(mouseY - playerY, mouseX - playerX);
	this->facing = rads * 180.0000 / 3.1416;
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

void Player::update(int elapsedTime) {
	this->x += this->dx * elapsedTime;
	this->y += this->dy * elapsedTime;
	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->x, this->y, this->facing, &player_constants::PLAYER_CENTER);
}