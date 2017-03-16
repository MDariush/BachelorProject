/*
Map.h
Created by Martin Dariush Hansen, 2017-03-14
*/

#pragma once
#include <vector>

class Map
{
public:
	enum CellStatus { OPEN, CLOSED };

	Map();
	~Map();
	void Init(const char* mapName);
	void LoadMapImage();
	bool getMapLoaded();
	unsigned int getWidth();
	unsigned int getHeight();
	std::vector<std::vector<CellStatus>> getCellStatusArray();

private:
	bool loaded;
	const char* name;
	unsigned int width;
	unsigned int height;
	std::vector<std::vector<CellStatus>> cellStatusArray;
};

