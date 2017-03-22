/*
Player.h
Created by Martin Dariush Hansen, 2017-03-22
*/

#pragma once
#include "Vision.h"
#include <vector>

class Player
{
public:
	Player();
	~Player();
	void Init(signed int mapWidth0, signed int mapHeight0);
	void Step();
	void AddUnit(class Unit* unit0);
	std::vector<std::vector<Vision::VisionStatus>>* getVisionArrayPtr();

private:
	Vision vision;

	std::vector<class Unit*> unitsPtr;
};
