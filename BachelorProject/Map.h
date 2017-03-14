/*
Map.h
Created by Martin Dariush Hansen, 2017-03-14
*/

#pragma once
#include <vector>
using namespace std;

class Map
{
public:
	enum CellStatus { OPEN, CLOSED };

	Map();
	~Map();
	void Init(const char* mapName);
	void LoadMapImage();
	bool getMapLoaded();

private:
	bool mapLoaded;
	const char* mapName;
	
	std::vector<CellStatus> mapArray;
	unsigned int mapWidth;
	unsigned int mapHeight;
};

