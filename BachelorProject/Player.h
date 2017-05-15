/*
Player.h
Created by Martin Dariush Hansen, 2017-03-22
*/

#pragma once
#include "Map.h"
#include "Pathfinder.h"
#include "Vision.h"
#include <vector>

class Player
{
public:
	Player();
	~Player();
	void Init(int playerNumberArg, class Map* pMapArg);
	void Step();
	void CreateUnit(double xArg, double yArg, class Map* pMapArg);
	void RemoveUnit(int indexArg);
	void UpdateUnits();
	void RemoveDeadUnits();
	std::vector<class Unit>* getUnitsPtr();
	std::vector<std::vector<Vision::VisionStatus>>* getVisionArrayPtr();
	Pathfinder * getPathfinderPtr();
	Vision * getVisionPtr();

private:
	Pathfinder pathfinder;
	Vision vision;

	std::vector<class Unit> units;
	int playerNumber;
};
