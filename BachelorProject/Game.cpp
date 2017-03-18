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

	unitsCreated = 0;
}

void Game::Step() {

	// Create unit at random location
	if (unitsCreated < UNITS_TO_CREATE && units.size() < MAX_UNITS_ON_MAP) {

		long double randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0)) + 0.5;
		long double randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0)) + 0.5;

		while (pMap->getCellStatus(randomX, randomY) != Map::OPEN) {
			randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0)) + 0.5;
			randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0)) + 0.5;
		}

		createUnit(randomX, randomY);
	}
}

void Game::createUnit(long double x0, long double y0) {
	units.push_back(Unit());
	units.at(units.size() - 1).Init(x0, y0);
	cout << "Unit created at (" << units.at(units.size()-1).getX() << ", " << units.at(units.size() - 1).getY() << ").\n";
}

std::vector<class Unit>* Game::getUnitsPtr() {
	return &units;
}
