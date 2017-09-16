#pragma once

#include <map>
#include <string>
#include "SDL.h"

//struct SDL_Window;
//struct SDL_Renderer;

class Graphics {
public:
	Graphics();
	~Graphics();

	// Load an image into the spriteSheets map if it doesn't already exist
	// Returns the image from the map
	SDL_Surface* loadImage(const std::string &filePath);

	// Draws a texture to a certain part of the screen
	void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle, float angle = 0, SDL_Point* center = NULL);

	// Renders everything to the screen
	void flip();

	// Clears the screen
	void clear();

	// Returns the renderer
	SDL_Renderer* getRenderer() const;
private:
	SDL_Window *window;
	SDL_Renderer *renderer;

	std::map<std::string, SDL_Surface*> spriteSheets;
};
