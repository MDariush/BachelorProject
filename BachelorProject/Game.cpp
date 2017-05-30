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
	terrainChangeTimer = -1;
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
	if (DYNAMIC_TERRAIN) {
		int randomX, randomY;

		// Change a random cell when initializing
		while (terrainChangeTimer == -1) {
			randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
			randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));

			if ((float)rand() / RAND_MAX < 0.5) {
				if (pMap->getCellStatus(randomX, randomY) == Map::OPEN
					&& pMap->CellTouchingCellOfType(Map::CLOSED, randomX, randomY)
					&& !pMap->CellBlocking(randomX, randomY)
					&& !UnitsExistInRect(randomX - 1, randomY - 1, randomX + 2, randomY + 2)) {

					pMap->setCellStatus(Map::CLOSED, randomX, randomY);
					terrainChangeTimer = 0;
					previousTerrainChangeCreatedABlock = true;
				}
			}
			else {
				if (pMap->getCellStatus(randomX, randomY) == Map::CLOSED
					&& pMap->CellTouchingCellOfType(Map::OPEN, randomX, randomY)) {

					pMap->setCellStatus(Map::OPEN, randomX, randomY);
					terrainChangeTimer = 0;
					previousTerrainChangeCreatedABlock = false;
				}
			}
		}

		// Create obstacle whithout blocking areas
		if (terrainChangeTimer >= terrainChangeTime) {
			if (previousTerrainChangeCreatedABlock == false) {
				for (int i = 0; i < DYNAMIC_TERRAIN_CLUSTER_RETRIES; i++) {
					randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
					randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));

					if (pMap->getCellStatus(randomX, randomY) == Map::OPEN
						&& pMap->CellTouchingCellOfType(Map::CLOSED, randomX, randomY)
						&& !pMap->CellBlocking(randomX, randomY)
						&& !UnitsExistInRect(randomX - 1, randomY - 1, randomX + 2, randomY + 2)) {

						pMap->setCellStatus(Map::CLOSED, randomX, randomY);
						previousTerrainChangeCreatedABlock = true;
						break;
					}
				}
			}

			// Remove obstacle near open space
			else {
				for (int i = 0; i < DYNAMIC_TERRAIN_CLUSTER_RETRIES; i++) {
					randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
					randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));

					if (pMap->getCellStatus(randomX, randomY) == Map::CLOSED
						&& pMap->CellTouchingCellOfType(Map::OPEN, randomX, randomY)) {

						pMap->setCellStatus(Map::OPEN, randomX, randomY);
						previousTerrainChangeCreatedABlock = false;
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

bool Game::UnitsExistInRect(double x0Arg, double y0Arg, double x1Arg, double y1Arg) {
	for (int i = 0; i < players.size(); i++) {
		if (players.at(i).HasUnitsInRect(x0Arg, y0Arg, x1Arg, y1Arg)) {
			return true;
		}
	}
	return false;
}

bool Game::getInitialized() {
	return initialized;
}

std::vector<class Player>* Game::getPlayersPtr() {
	return &players;
}
