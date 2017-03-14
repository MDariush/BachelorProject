/*
Map.cpp
Created by Martin Dariush Hansen, 2017-03-14
*/

#include "lodepng.h"
#include "Map.h"
#include <iostream>
using namespace std;

Map::Map() {
}

Map::~Map() {
}

void Map::Init(const char* mapName0) {
	mapName = mapName0;
	mapLoaded = false;
}

void Map::LoadMapImage() {
	std::vector<unsigned char> mapVector;
	unsigned int error = lodepng::decode(mapVector, mapWidth, mapHeight, mapName);
	
	if (error) {
		cout << "Unable to load map image. Decoder error " << error << ": " << lodepng_error_text(error) << "\n";
	}
	else {
		// Create map array from map vector
		CellStatus** cellStatusArray = new CellStatus*[mapWidth];
		for (int i = 0; i < mapWidth; ++i) {
			cellStatusArray[i] = new CellStatus[mapHeight];
		}
		for (int i = 0; i < mapWidth; ++i) {
			for (int j = 0; j < mapHeight; ++j) {
				if (mapVector.at(mapWidth*(j)+i) == 0 && mapVector.at(mapWidth*(j)+i + 1) == 0 && mapVector.at(mapWidth*(j)+i + 2) == 0) {
					cellStatusArray[i][j] = CellStatus::CLOSED;
				}
				else {
					cellStatusArray[i][j] = CellStatus::OPEN;
				}
			}
		}

		mapLoaded = true;
		cout << "Map image loaded.";
	}
}

bool Map::getMapLoaded() {
	return mapLoaded;
}
