/*
Player.h
Created by Martin Dariush Hansen, 2017-03-22
*/

#pragma once
#include "Map.h"
#include "Vision.h"
#include <vector>

class Player
{
public:
	Player();
	~Player();
	void Init(int playerNumberArg, std::vector<std::vector<Map::CellStatus>>* pCellStatusArray, unsigned int mapWidthArg, unsigned int mapHeightArg);
	void Step();
	void CreateUnit(long double xArg, long double yArg);
	void UpdateUnitMovement();
	std::vector<class Unit>* getUnitsPtr();
	std::vector<std::vector<Vision::VisionStatus>>* getVisionArrayPtr();

private:
	Vision vision;

	std::vector<class Unit> units;
	int playerNumber;
};
