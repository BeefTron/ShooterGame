#pragma once

#include <SDL.h>
#include <map>

class Input {
public:
	// Called at the beginning of each new frame
	// to reset the keys that are no longer relevant
	void beginNewFrame();
	void keyUpEvent(const SDL_Event& event);
	// Called when a key has been pressed
	void keyDownEvent(const SDL_Event& event);

	bool isKeyHeld(SDL_Scancode key);
	bool wasKeyPressed(SDL_Scancode key);
	bool wasKeyReleased(SDL_Scancode key);
private:
	std::map<SDL_Scancode, bool> heldKeys;
	std::map<SDL_Scancode, bool> pressedKeys;
	std::map<SDL_Scancode, bool> releasedKeys;
};