#include "animatedsprite.h"
#include "graphics.h"
#include "sprite.h"

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY, long timeToUpdate) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
	frameIndex(0),
	timeToUpdate(timeToUpdate),
	timeElapsed(0),
	visible(true),
	currentAnimationOnce(false),
	currentAnimation("")
{}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset) {
	std::vector<SDL_Rect> rectangles;
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { (i * width) + x, y, width, height };
		rectangles.push_back(newRect);
	}
	this->animations.insert(std::pair<std::string, std::vector<SDL_Rect> >(name, rectangles));
	this->offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::resetAnimations() {
	this->animations.clear();
	this->offsets.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
	this->currentAnimationOnce = once;
	if (this->currentAnimation != animation) {
		this->currentAnimation = animation;
		this->frameIndex = 0;
	}
}

void AnimatedSprite::setVisible(bool visible) {
	this->visible = visible;
}

void AnimatedSprite::stopAnimation() {
	this->frameIndex = 0;
	this->animationDone(this->currentAnimation);
}

void AnimatedSprite::update(int elapsedTime) {
	Sprite::update();

	this->timeElapsed += elapsedTime;
	if (this->timeElapsed > this->timeToUpdate) {
		this->timeElapsed -= timeToUpdate;
		if (this->frameIndex < this->animations[this->currentAnimation].size() - 1) {
			this->frameIndex++;
		} else {
			if (currentAnimationOnce == true) {
				this->setVisible(false);
			}
			this->frameIndex = 0;
			this->animationDone(this->currentAnimation);
		}
	}
}

void AnimatedSprite::draw(Graphics &graphics, float x, float y, float angle, SDL_Point* center) {
	if (this->visible) {
		SDL_Rect destinationRectangle;
		destinationRectangle.x = x + this->offsets[this->currentAnimation].x;
		destinationRectangle.y = y + this->offsets[this->currentAnimation].y;
		destinationRectangle.w = this->sourceRect.w * globals::SPRITE_SCALE;
		destinationRectangle.h = this->sourceRect.h * globals::SPRITE_SCALE;

		SDL_Rect sourceRect = this->animations[this->currentAnimation][this->frameIndex];
		graphics.blitSurface(this->spriteSheet, &sourceRect, &destinationRectangle, angle, center);
	}
}