/*
Game.cpp
Created by Martin Dariush Hansen, 2017-03-17
*/

#include "Configurations.h"
#include "Game.h"
#include "Map.h"
#include "Unit.h"
#include <iostream>
using namespace std;

Game::Game() {
}

Game::~Game() {
}

void Game::Init(class Map* pMap0) {
	pMap = pMap0;
}

void Game::Step() {

	// Create unit at random location
	if (units.size() < MAX_UNITS_ON_MAP) {

		unsigned int randomX = int((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
		unsigned int randomY = int((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));

		while (pMap->getCellStatus(randomX, randomY) != Map::OPEN) {
			randomX = int((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
			randomY = int((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));
		}

		createUnit(randomX, randomY);
	}
}

void Game::createUnit(long double x0, long double y0) {
	Unit unit;
	units.push_back(&unit);
	cout << "Unit created in cell at (" << x0 << ", " << y0 << ").\n";
}
