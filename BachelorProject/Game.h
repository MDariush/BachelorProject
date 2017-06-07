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
	void Init(class Map* pMap0, class Timer* pTimerArg);
	void Step();
	void CreatePlayer();
	bool UnitsExistInRect(double x0Arg, double y0Arg, double x1Arg, double y1Arg);
	bool getInitialized();

	std::vector<class Player>* getPlayersPtr();

private:
	std::vector<class Player> players;
	class Map* pMap;
	class Timer* pTimer;

	bool initialized;
	int unitsCreated;
	int terrainChangeTimer;
	int terrainChangeTime;
	bool previousTerrainChangeCreatedABlock;
};

