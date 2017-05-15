/*
Map.cpp
Created by Martin Dariush Hansen, 2017-03-14
*/

#include "lodepng.h"
#include "Map.h"
#include <iostream>
using namespace std;

const vector<pair<int, int>> Map::cellSurroundChecking = {
	std::make_pair(1, 0),
	std::make_pair(1, 1),
	std::make_pair(0, 1),
	std::make_pair(-1, 1),
	std::make_pair(-1, 0),
	std::make_pair(-1, -1),
	std::make_pair(0, -1),
	std::make_pair(1, -1)
};

Map::Map() {
	initialized = false;
	
}

Map::~Map() {
}

void Map::Init(const char* mapName0) {
	name = mapName0;

	LoadMapImage();

	initialized = true;
	generation = 0;
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
		int offsetPerPixel = 4;
		int offsetRed = 0;
		int offsetGreen = 1;
		int offsetBlue = 2;

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (mapVector.at((width * j + i) * offsetPerPixel + offsetRed) == 0 
					&& mapVector.at((width * j + i) * offsetPerPixel + offsetGreen) == 0
					&& mapVector.at((width * j + i) * offsetPerPixel + offsetBlue) == 0) {

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

bool Map::CellBlocking(int xArg, int yArg) {
	bool openCell = false;
	bool closedAfterOpenCell = false;

	for (int i = 0; i < cellSurroundChecking.size(); i++) {
		if (IsLegalCell(cellSurroundChecking.at(i).first, cellSurroundChecking.at(i).second)
			&& cellStatusArray[cellSurroundChecking.at(i).first][cellSurroundChecking.at(i).second] == Map::OPEN) {

			if (closedAfterOpenCell) {
				return true;
			}
			openCell = true;
		}
		else if (openCell) {
			closedAfterOpenCell = true;
		}
	}

	return false;
}

bool Map::CellTouchingCellOfType(CellStatus typeArg, int xArg, int yArg) {
	return (xArg > 0 && getCellStatus(xArg - 1, yArg) == typeArg)
		|| (xArg < getWidth() - 1 && getCellStatus(xArg + 1, yArg) == typeArg)
		|| (yArg > 0 && getCellStatus(xArg, yArg - 1) == typeArg)
		|| (yArg < getHeight() - 1 && getCellStatus(xArg, yArg + 1) == typeArg);
}

void Map::Clear(CellStatus cellStatusArg) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			cellStatusArray[i][j] = cellStatusArg;
		}
	}

	cout << "Map cleared" << endl;

	generation++;
}

// #TODO: Unused?
void Map::Reset(int widthArg, int heightArg, CellStatus statusArg) {
	cellStatusArray.clear();
	cellStatusArray.resize(widthArg, std::vector<CellStatus>(heightArg, statusArg));
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
	generation++;
}

int Map::getGeneration() {
	return generation;
}

void Map::IncrementGeneration() {
	generation++;
}

std::vector<std::vector<Map::CellStatus>>* Map::getCellStatusArrayPtr() {
	return &cellStatusArray;
}
