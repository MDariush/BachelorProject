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
	terrainChangeTimer = 0;
	terrainChangeTime = TERRAIN_TIMER_MAX;
	CreatePlayer();
	initialized = true;
}

void Game::Step() {

	// Create unit at random location
	if (unitsCreated < UNITS_TO_CREATE && players.at(0).getUnitsPtr()->size() < MAX_UNITS_ON_MAP) {

		double randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
		double randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));

		while (pMap->getCellStatus(randomX, randomY) != Map::OPEN) {
			randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
			randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));
		}

		randomX += 0.5;
		randomY += 0.5;

		players.at(0).CreateUnit(randomX, randomY, pMap);
		unitsCreated++;
	}

	// Create obstacle next to other obstacles
	if (DYNAMIC_TERRAIN && terrainChangeTimer >= terrainChangeTime) {

		int randomX = 0;
		int randomY = 0;

		for (int i = 0; i < DYNAMIC_TERRAIN_CLUSTER_RETRIES + 1; i++) {
			randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
			randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));

			if (pMap->getCellStatus(randomX, randomY) == Map::OPEN
				&& pMap->CellTouchingCellOfType(Map::CLOSED, randomX, randomY)) {

				break;
			}
		}

		pMap->setCellStatus(Map::CLOSED, randomX, randomY);
		terrainChangeTimer = 0;
		terrainChangeTime = floor(((TERRAIN_TIMER_MAX - TERRAIN_TIMER_MIN) * rand()) / (RAND_MAX + 1.0)) + TERRAIN_TIMER_MIN;
	}
	else {
		terrainChangeTimer++;
	}

	// Update units
	for (int i = 0; i < players.size(); i++) {
		players.at(i).UpdateUnits();
	}

	// Remove dead units
	for (int i = 0; i < players.size(); i++) {
		players.at(i).RemoveDeadUnits();
	}

	// Update players
	for (int i = 0; i < players.size(); i++) {
		players.at(i).Step();
	}
}

void Game::CreatePlayer() {
	players.push_back(Player());
	players.at(players.size() - 1).Init(players.size() - 1, pMap);
	cout << "Player " << players.size() - 1 << " created.\n";
}

bool Game::getInitialized() {
	return initialized;
}

std::vector<class Player>* Game::getPlayersPtr() {
	return &players;
}
