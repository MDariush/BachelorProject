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
	/*if (IsLegalCell(xArg, yArg) && cellStatusArray[xArg][yArg] == Map::OPEN) {
		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				if (IsLegalCell(xArg + i, yArg + j)
					&& cellStatusArray[xArg + i][yArg + j] == Map::OPEN
					&& cellStatusArray[xArg + i][yArg] == Map::OPEN
					&& (cellStatusArray[xArg][yArg + j] == Map::OPEN
						|| (IsLegalCell(xArg + i, yArg - j)
							&& cellStatusArray[xArg + i][yArg - j] == Map::OPEN))) {

					return true;
				}
			}
		}
	}
	return false;*/
	
	int openCellsAfterClosedCells = 0;
	Map::CellStatus cellStatusPrevious;

	int x = xArg + cellSurroundChecking.at(cellSurroundChecking.size() - 1).first;
	int y = yArg + cellSurroundChecking.at(cellSurroundChecking.size() - 1).second;
	if (IsLegalCell(x, y)) {
		cellStatusPrevious = cellStatusArray[x][y];
	}
	else {
		cellStatusPrevious = Map::CLOSED;
	}
	Map::CellStatus cellStatus = cellStatusPrevious;

	for (int i = 0; i < cellSurroundChecking.size(); i++) {
		x = xArg + cellSurroundChecking.at(i).first;
		y = yArg + cellSurroundChecking.at(i).second;
		cellStatusPrevious = cellStatus;
		if (IsLegalCell(x, y)) {
			cellStatus = cellStatusArray[x][y];
			if (cellStatus == Map::OPEN && cellStatusPrevious == Map::CLOSED) {
				openCellsAfterClosedCells++;
				if (openCellsAfterClosedCells > 1) {
					return true;
				}
			}
		}
		else {
			cellStatus = Map::CLOSED;
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
