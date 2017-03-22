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
	if (unitsCreated < UNITS_TO_CREATE && units.size() < MAX_UNITS_ON_MAP) {

		long double randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0)) + 0.5;
		long double randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0)) + 0.5;

		while (pMap->getCellStatus(randomX, randomY) != Map::OPEN) {
			randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0)) + 0.5;
			randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0)) + 0.5;
		}

		CreateUnit(0, randomX, randomY);
	}
}

void Game::CreatePlayer() {
	players.push_back(Player());
	players.at(players.size() - 1).Init(pMap->getWidth(), pMap->getHeight());
	cout << "Player created.\n";
}

void Game::CreateUnit(unsigned int player0, long double x0, long double y0) {
	units.push_back(Unit());
	units.at(units.size() - 1).Init(player0, x0, y0);
	players.at(player0).AddUnit(&units.at(units.size() - 1));
	cout << "Unit created for Player " << player0 << " at (" << units.at(units.size()-1).getX() << ", " << units.at(units.size() - 1).getY() << ").\n";
}

bool Game::getInitialized() {
	return initialized;
}

std::vector<class Player>* Game::getPlayersPtr() {
	return &players;
}

std::vector<class Unit>* Game::getUnitsPtr() {
	return &units;
}
