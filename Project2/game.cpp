#include <SDL.h>
#include <algorithm>
#include "game.h"
#include "graphics.h"
#include "input.h"

namespace {
	const int FPS = 60;
	const int MAX_FRAME_TIME = 6 * 1000 / FPS;
}

Game::Game() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}
	this->gameLoop();
}

Game::~Game() {

}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	this->level = Level("street", Vector2(100, 100), graphics);
	this->player = Player(graphics, this->level.getPlayerSpawn());

	int LAST_UPDATE_TIME = SDL_GetTicks();

	// Start the game loop
	while (true) {
		input.beginNewFrame();

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			return;
		}

		int mouseX, mouseY;
		int mouseState = SDL_GetMouseState(&mouseX, &mouseY);
		this->player.setFacing(mouseX, mouseY);
		if (mouseState == 1) {
			this->player.shoot();
		}
		else {
			this->player.holdFire();
		}

		int horizontal = 0;
		int vertical = 0;
		if (input.isKeyHeld(SDL_SCANCODE_A)) {
			horizontal -= 1;
		}
		if (input.isKeyHeld(SDL_SCANCODE_D)) {
			horizontal += 1;
		}
		if (input.isKeyHeld(SDL_SCANCODE_W)) {
			vertical -= 1;
		}
		if (input.isKeyHeld(SDL_SCANCODE_S)) {
			vertical += 1;
		}
		if (!horizontal && !vertical) {
			this->player.stopMoving();
		}
		else {
			if (input.isKeyHeld(SDL_SCANCODE_LSHIFT)) {
				this->player.move(horizontal, vertical, 2.0);
			}
			else if (input.isKeyHeld(SDL_SCANCODE_LCTRL)) {
				this->player.move(horizontal, vertical, 0.5);
			}
			else {
				this->player.move(horizontal, vertical);
			}
		}
		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::draw(Graphics &graphics) {
	graphics.clear();

	this->level.draw(graphics);
	this->player.draw(graphics);

	graphics.flip();
}

void Game::update(int elapsedTime) {
	this->player.update(elapsedTime);
	this->level.update(elapsedTime);

	// Check collisions
	std::vector<Rectangle> others;
	if ((others = this->level.checkTileCollisions(this->player.getBoundingBox())).size() > 0) {
		// Player collided with at least one tile
		this->player.handleTileCollisions(others);
	}
}