#pragma once

#include <map>
#include <string>
#include <vector>
#include "sprite.h"
#include "globals.h"

class Graphics;

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, long timeToUpdate);
	// Plays the animation if it's not already playing
	// if once, will only play once
	void playAnimation(std::string animation, bool once = false);

	void update(int elapsedTime);
	
	void draw(Graphics &graphics, float x, float y, float angle = 0, SDL_Point* center = NULL);
protected:
	long timeToUpdate;
	bool currentAnimationOnce;
	std::string currentAnimation;

	// Adds an animation to the map of animations for the sprite
	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	// Resets all animations associated with this sprite
	void resetAnimations();

	void stopAnimation();

	void setVisible(bool visible);

	// Logic that happens when an animation ends
	virtual void animationDone(std::string currentAnimation) = 0;

	// A required function that sets up all animations for a sprite
	virtual void setupAnimation() = 0;
private:
	std::map<std::string, std::vector<SDL_Rect> > animations;
	std::map<std::string, Vector2> offsets;

	unsigned frameIndex;
	unsigned long timeElapsed;
	bool visible;
};