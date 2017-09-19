#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "globals.h"

Graphics::Graphics() {
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH , globals::SCREEN_HEIGHT, 0, &this->window, &this->renderer);
	SDL_SetWindowTitle(this->window, "GAME");
}

Graphics::~Graphics() {
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
	if (this->spriteSheets.count(filePath) == 0) {
		this->spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return this->spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle, float angle, const SDL_Point* center) {
	SDL_RenderCopyEx(this->renderer, source, sourceRectangle, destinationRectangle, angle, center, SDL_FLIP_NONE);
}

void Graphics::flip() {
	SDL_RenderPresent(this->renderer);
}

void Graphics::clear() {
	SDL_RenderClear(this->renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
	return this->renderer;
}