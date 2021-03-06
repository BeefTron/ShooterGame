#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "SDL.h"
#include "tinyxml2.h"
#include "player.h"
#include "enemy.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <utility>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Graphics &graphics) :
	mapName(mapName),
	size(Vector2()) 
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {}

void Level::loadMap(std::string mapName, Graphics &graphics) {
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
			int firstGid, tileCount;
			std::string path = pTileset->FirstChildElement("image")->Attribute("source");
			path.erase(0, 3); // Because source gives the path to the tileset from the .tmx file, not this dir
			std::stringstream ss;
			ss << path;
			pTileset->QueryIntAttribute("firstgid", &firstGid);
			pTileset->QueryIntAttribute("tilecount", &tileCount);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			this->tilesets.push_back(Tileset(tex, firstGid, firstGid + tileCount - 1));

			// Get animations for that tileset
			XMLElement* pTileA = pTileset->FirstChildElement("tile");
			if (pTileA != NULL) {
				while (pTileA) {
					AnimatedTileInfo ati;
					ati.StartTileId = pTileA->IntAttribute("id") + firstGid;
					ati.TilesetsFirstGid = firstGid;
					XMLElement* pAnimation = pTileA->FirstChildElement("animation");
					if (pAnimation != NULL) {
						while (pAnimation) {
							XMLElement* pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstGid);
									ati.Durations.push_back(pFrame->IntAttribute("duration"));

									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->animatedTileInfos.push_back(ati);

					pTileA = pTileA->NextSiblingElement("tile");
				}
			}
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
								if (this->tilesets[i].FirstGid <= gid && this->tilesets[i].LastGid >= gid) {
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

							// Build the actual tile and add it to the level's tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < this->animatedTileInfos.size(); i++) {
								if (this->animatedTileInfos.at(i).StartTileId == gid) {
									ati = this->animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile) {
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); i++) {
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i), tileWidth, tileHeight));
								}
								AnimatedTile tile(tilesetPositions, ati.Durations, tls.Texture, Vector2(tileWidth, tileHeight), finalTilePos);
								this->animatedTileList.push_back(tile);
							}
							else {
								// Calculate the position of the tile in the tileset
								Vector2 finalTilesetPos = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);
								Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPos, finalTilePos);
								this->tileList.push_back(tile);
							}
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

	// Parse out the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "spawnPoints") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						this->spawnPointMap[ss.str()] = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y) * globals::SPRITE_SCALE);
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "doors") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle rect = Rectangle(x, y, w, h);

						const char* dest = pObject->Attribute("name");
						std::stringstream ss;
						ss << dest;
						Door door = Door(rect, ss.str());
						this->doorList.push_back(door);

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "enemies") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* type = pObject->Attribute("name");
						std::stringstream ss;
						ss << type;
						if (ss.str() == "alien") {
							this->enemies.push_back(new Alien(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE)));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

void Level::update(int elapsedTime, Player &player) {
	for (int i = 0; i < this->animatedTileList.size(); i++) {
		this->animatedTileList.at(i).update(elapsedTime);
	}
	Vector2 playerPos = Vector2(player.getCenterX(), player.getCenterY());
	for (int i = 0; i < this->enemies.size(); i++) {
		Vector2 enemyPos = Vector2(this->enemies.at(i)->getCenterX(), this->enemies.at(i)->getCenterY());
		bool inSight = this->BresenhamSight(playerPos, enemyPos);
		this->enemies.at(i)->setInSight(inSight);
		if (inSight && player.isShooting() && player.getHealth() > 0) {
			player.checkBulletCollision(enemies.at(i));
		}
		this->enemies.at(i)->update(elapsedTime, player);
	}
}

void Level::draw(Graphics &graphics) {
	for (int i = 0; i < this->tileList.size(); i++) {
		this->tileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->animatedTileList.size(); i++) {
		this->animatedTileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->enemies.size(); i++) {
		this->enemies.at(i)->draw(graphics);
	}
}

Door Level::checkDoorCollisions(const Rectangle &playerBox) {
	for (int i = 0; i < this->doorList.size(); i++) {
		if (this->doorList.at(i).collidesWith(playerBox)) {
			return this->doorList.at(i);
		}
	}
	return Door();
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
	std::vector<Rectangle> collidingRects;
	for (int i = 0; i < this->collisionRects.size(); i++) {
		if (this->collisionRects.at(i).collidesWith(other)) {
			collidingRects.push_back(this->collisionRects.at(i));
		}
	}
	return collidingRects;
}

std::vector<Enemy*> Level::checkEnemyCollisions(const Rectangle &other) {
	std::vector<Enemy*> collidingEnemies;
	for (int i = 0; i < this->enemies.size(); i++) {
		if (this->enemies.at(i)->getBoundingBox().collidesWith(other)) {
			collidingEnemies.push_back(this->enemies.at(i));
		}
	}
	return collidingEnemies;
}

bool Level::checkPointCollisions(int x, int y) const {
	for (int i = 0; i < this->collisionRects.size(); i++) {
		if (this->collisionRects.at(i).containsPoint(x, y)) {
			return true;
		}
	}
	return false;
}

bool Level::BresenhamSight(Vector2 p1, Vector2 p2) const {
	bool steep = abs(p2.y - p1.y) > abs(p2.x - p1.x);
	if (steep) {
		std::swap(p1.x, p1.y);
		std::swap(p2.x, p2.y);
	}
	if (p1.x > p2.x) {
		std::swap(p1.x, p2.x);
		std::swap(p1.y, p2.y);
	}
	int dx = p2.x - p1.x;
	int dy = abs(p2.y - p1.y);
	int err = 0;
	int ystep;
	int y = p1.y;
	if (p1.y < p2.y) {
		ystep = 1;
	}
	else {
		ystep = -1;
	}
	for (int x = p1.x; x <= p2.x; x++) {
		if (steep) {
			if (this->checkPointCollisions(y, x)) {
				return false;
			}
		}
		else {
			if (this->checkPointCollisions(x, y)) {
				return false;
			}
		}
		err += dy;
		if (2 * err >= dx) {
			y += ystep;
			err -= dx;
		}
	}
	return true;
}

Vector2 Level::getPlayerSpawn(std::string from) {
	if (from == "" || this->spawnPointMap.find(from) == this->spawnPointMap.end()) {
		// from is not an existing key in spawnPointMap. Just use default spawn
		return this->spawnPointMap["default"];
	}
	else {
		return this->spawnPointMap[from];
	}	
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	// Adjust the gid range to start at 0 for each tileset
	gid -= tls.FirstGid;
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = gid % (tilesetWidth / tileWidth);
	tsxx *= tileWidth;
	int tsyy = gid / (tilesetWidth / tileWidth);
	tsyy *= tileHeight;
	return Vector2(tsxx, tsyy);
}