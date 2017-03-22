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
}

void Map::LoadMapImage() {
	std::vector<unsigned char> mapVector;
	unsigned int error = lodepng::decode(mapVector, width, height, name);
	
	if (error) {
		cout << "Unable to load map image. Decoder error " << error << ": " << lodepng_error_text(error) << "\n";
	}
	else {
		// Set cell status array size
		cellStatusArray.resize(width);
		for (int i = 0; i < width; i++) {
			cellStatusArray[i].resize(height);
		}

		// Set cell status array values
		for (int i = 0; i < width; i++) {
			cellStatusArray.push_back(vector<CellStatus>());
			for (int j = 0; j < height; j++) {
				if (mapVector.at((width*(j)+i) * 4) == 0 && mapVector.at((width*(j)+i) * 4 + 1) == 0 && mapVector.at((width*(j)+i) * 4 + 2) == 0) {
					cellStatusArray[i][j] = CellStatus::CLOSED;
				}
				else {
					cellStatusArray[i][j] = CellStatus::OPEN;
				}
			}
		}

		cout << "Map image loaded.\n";
	}
}

bool Map::getInitialized() {
	return initialized;
}

unsigned int Map::getWidth() {
	return width;
}

unsigned int Map::getHeight() {
	return height;
}

Map::CellStatus Map::getCellStatus(signed int i, signed int j) {
	return cellStatusArray[i][j];
}

std::vector<std::vector<Map::CellStatus>> Map::getCellStatusArray() {
	return cellStatusArray;
}
