/*
Game.cpp
Created by Martin Dariush Hansen, 2017-03-17
*/

#include "Configurations.h"
#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "Unit.h"
#include <iostream>
using namespace std;

Game::Game() {
	initialized = false;
}

Game::~Game() {
}

void Game::Init(class Map* pMap0) {
	pMap = pMap0;
	unitsCreated = 0;

	CreatePlayer();
	initialized = true;
}

void Game::Step() {

	// Create unit at random location
	if (unitsCreated < UNITS_TO_CREATE && players.at(0).getUnitsPtr()->size() < MAX_UNITS_ON_MAP) {

		long double randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
		long double randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));

		while (pMap->getCellStatus(randomX, randomY) != Map::OPEN) {
			randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
			randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));
		}

		randomX += 0.5;
		randomY += 0.5;

		players.at(0).CreateUnit(randomX, randomY);
		unitsCreated++;
	}

	// Update units
	for (int i = 0; i < players.size(); i++) {
		players.at(i).UpdateUnitMovement();
	}

	// Update players
	for (int i = 0; i < players.size(); i++) {
		players.at(i).Step();
	}
}

void Game::CreatePlayer() {
	players.push_back(Player());
	players.at(players.size() - 1).Init(players.size() - 1, pMap->getCellStatusArrayPtr(), pMap->getWidth(), pMap->getHeight());
	cout << "Player " << players.size() - 1 << " created.\n";
}

bool Game::getInitialized() {
	return initialized;
}

std::vector<class Player>* Game::getPlayersPtr() {
	return &players;
}
