/*
Map.cpp
Created by Martin Dariush Hansen, 2017-03-14
*/

#include "lodepng.h"
#include "Map.h"
#include <iostream>
using namespace std;

Map::Map() {
	initialized = false;
}

Map::~Map() {
}

void Map::Init(const char* mapName0) {
	name = mapName0;

	LoadMapImage();

	initialized = true;
	mapUpdated = true;
}

void Map::LoadMapImage() {
	std::vector<unsigned char> mapVector;
	unsigned int error = lodepng::decode(mapVector, width, height, name);
	
	if (error) {
		cout << "Unable to load map image. Decoder error " << error << ": " << lodepng_error_text(error) << "\n";
	}
	else {
		// Set cell status array size
		cellStatusArray.resize(width, std::vector<CellStatus>(height, CellStatus::OPEN));

		// Set cell status array values
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (mapVector.at((width*(j)+i) * 4) == 0 && mapVector.at((width*(j)+i) * 4 + 1) == 0 && mapVector.at((width*(j)+i) * 4 + 2) == 0) {
					cellStatusArray[i][j] = CellStatus::CLOSED;
				}
			}
		}
		cout << "Map image (" << cellStatusArray.size() << "x" << cellStatusArray.at(0).size() << ") loaded.\n";
	}
}

bool Map::IsLegalCell(int x, int y) {
	return x >= 0 && x < cellStatusArray.size() && y >= 0 && y < cellStatusArray.at(0).size();
}

bool Map::CellTouchingCellOfType(CellStatus typeArg, int xArg, int yArg) {
	return (xArg > 0 && getCellStatus(xArg - 1, yArg) == typeArg)
		|| (xArg < getWidth() - 1 && getCellStatus(xArg + 1, yArg) == typeArg)
		|| (yArg > 0 && getCellStatus(xArg, yArg - 1) == typeArg)
		|| (yArg < getHeight() - 1 && getCellStatus(xArg, yArg + 1) == typeArg);
}

bool Map::getInitialized() {
	return initialized;
}

int Map::getWidth() {
	return width;
}

int Map::getHeight() {
	return height;
}

Map::CellStatus Map::getCellStatus(int xArg, int yArg) {
	return cellStatusArray[xArg][yArg];
}

void Map::setCellStatus(CellStatus cellStatusArg, int xArg, int yArg) {
	cellStatusArray[xArg][yArg] = cellStatusArg;
	mapUpdated = true;
}

bool Map::getMapUpdated() {
	return mapUpdated;
}

void Map::setMapUpdated(bool mapUpdatedArg) {
	mapUpdated = mapUpdatedArg;
}

std::vector<std::vector<Map::CellStatus>>* Map::getCellStatusArrayPtr() {
	return &cellStatusArray;
}
