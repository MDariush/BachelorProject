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
	bool getInitialized();

	std::vector<class Player>* getPlayersPtr();

private:
	std::vector<class Player> players;
	
	unsigned int unitsCreated;

	class Map* pMap;

	bool initialized;
};

