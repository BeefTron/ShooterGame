#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "SDL.h"
#include "tinyxml2.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) :
	mapName(mapName),
	spawnPoint(spawnPoint),
	size(Vector2()) 
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {}

void Level::loadMap(std::string mapName, Graphics &graphics) {
	// Temp code to load map
	//this->backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("backgrounds/tile.png"));
	//this->size = Vector2(globals::SCREEN_WIDTH * 2, globals::SCREEN_HEIGHT * 2);

	// Parse .tmx file
	XMLDocument doc;
	std::stringstream mapSource;
	mapSource << "maps/" << mapName << ".tmx";
	if (doc.LoadFile(mapSource.str().c_str())) {
		printf("Unable to load Map File: %s\n", mapSource.str().c_str());
		return;
	}

	XMLElement* mapNode = doc.FirstChildElement("map");

	// Get width and height of the map and store in size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->size = Vector2(width, height);

	// Get width and height of the tiles and store in tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->tileSize = Vector2(tileWidth, tileHeight);

	// Load the tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		while (pTileset) {
			int firstGid;
			std::string path = pTileset->FirstChildElement("image")->Attribute("source");
			path.erase(0, 3); // Because source gives the path to the tileset from the .tmx file, not this dir
			std::stringstream ss;
			ss << path;
			pTileset->QueryIntAttribute("firstgid", &firstGid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			this->tilesets.push_back(Tileset(tex, firstGid));

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	// Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			// Loading data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					// Loading tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							// Build each individual tile here
							// if gid = 0, no tile should be drawn
							int gid = pTile->IntAttribute("gid");
							if (!gid) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}
							// Get the tileset for this gid
							Tileset tls;
							for (int i = 0; i < this->tilesets.size(); i++) {
								if (this->tilesets[i].FirstGid <= gid) {
									// This is the tileset we want
									tls = this->tilesets.at(i);
									break;
								}
							}

							if (tls.FirstGid == -1) {
								// No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							// Get the position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileWidth * (tileCounter % width);
							yy = tileHeight * (tileCounter / width);
							Vector2 finalTilePos = Vector2(xx, yy);

							// Calculate the position of the tile in the tileset
							int tilesetWidth, tilesetHeight;
							SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
							int tsxx = gid % (tilesetWidth / tileWidth) - 1;
							tsxx *= tileWidth;
							int tsyy = (gid / (tilesetWidth / tileWidth));
							tsyy *= tileHeight;
							Vector2 finalTilesetPos = Vector2(tsxx, tsyy);

							// Build the actual tile and add it to the level's tile list
							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPos, finalTilePos);
							this->tileList.push_back(tile);
							tileCounter++;

							pTile = pTile->NextSiblingElement("tile");
						}
					}
					pData = pData->NextSiblingElement("data");
				}
			}
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}
}

void Level::update(int elapsedTime) {}

void Level::draw(Graphics &graphics) {
	/*SDL_Rect sourceRect = { 0, 0, 64, 64 };
	SDL_Rect destRect;
	for (int x = 0; x < this->size.x / 64; x++) {
		for (int y = 0; y < this->size.y / 64; y++) {
			destRect.x = x * 64 * globals::SPRITE_SCALE;
			destRect.y = y * 64 * globals::SPRITE_SCALE;
			destRect.w = 64 * globals::SPRITE_SCALE;
			destRect.h = 64 * globals::SPRITE_SCALE;
			graphics.blitSurface(this->backgroundTexture, &sourceRect, &destRect);
		}
	}*/
	for (int i = 0; i < this->tileList.size(); i++) {
		this->tileList.at(i).draw(graphics);
	}
}