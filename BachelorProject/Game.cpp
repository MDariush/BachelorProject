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

	// Change terrain
	if (DYNAMIC_TERRAIN && terrainChangeTimer >= terrainChangeTime) {
		int randomX, randomY;
		
		// 50% chance to create obstacle whithout blocking areas
		if ((float) rand() / RAND_MAX < 0.5) {
			for (int i = 0; i < DYNAMIC_TERRAIN_CLUSTER_RETRIES; i++) {
				randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
				randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));

				if (pMap->getCellStatus(randomX, randomY) == Map::OPEN
					&& !pMap->CellBlocking(randomX, randomY)) {

					pMap->setCellStatus(Map::CLOSED, randomX, randomY);
					break;
				}
			}
		}

		// 50% chance to remove obstacle near open space
		else {
			for (int i = 0; i < DYNAMIC_TERRAIN_CLUSTER_RETRIES; i++) {
				randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
				randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));

				if (pMap->getCellStatus(randomX, randomY) == Map::CLOSED
					&& pMap->CellTouchingCellOfType(Map::OPEN, randomX, randomY)) {

					pMap->setCellStatus(Map::OPEN, randomX, randomY);
					break;
				}
			}
		}

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
