/*
Game.h
Created by Martin Dariush Hansen, 2017-03-17
*/

#pragma once
#include <vector>

class Game
{
public:
	Game();
	~Game();
	void Init(class Map* pMap0);
	void Step();
	void createUnit(long double x0, long double y0);

private:
	unsigned int unitsCreated = 0;
	std::vector<class Unit*> units;

	class Map* pMap;
};

