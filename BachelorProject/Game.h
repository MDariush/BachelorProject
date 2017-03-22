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
	void CreatePlayer();
	void CreateUnit(unsigned int player0, long double x0, long double y0);
	bool getInitialized();

	std::vector<class Player>* getPlayersPtr();
	std::vector<class Unit>* getUnitsPtr();

private:
	std::vector<class Player> players;
	std::vector<class Unit> units;
	unsigned int unitsCreated;

	class Map* pMap;

	bool initialized;
};

