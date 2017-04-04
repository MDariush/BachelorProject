/*
Player.cpp
Created by Martin Dariush Hansen, 2017-03-22
*/

#include "Player.h"
#include "Unit.h"
#include <iostream>
using namespace std;

Player::Player() {
}

Player::~Player() {
}

void Player::Init(int playerNumberArg, std::vector<std::vector<Map::CellStatus>>* pCellStatusArray, unsigned int mapWidthArg, unsigned int mapHeightArg) {
	playerNumber = playerNumberArg;
	cout << pCellStatusArray->size() << endl;
	vision.Init(pCellStatusArray);
}

void Player::Step() {
	vision.FullUpdate(&units);
}

void Player::CreateUnit(long double xArg, long double yArg) {
	units.push_back(Unit());
	units.at(units.size() - 1).Init(playerNumber, xArg, yArg);
	cout << "Unit created for Player " << playerNumber << " at (" << units.at(units.size() - 1).getX() << ", " << units.at(units.size() - 1).getY() << ").\n";
}

void Player::UpdateUnitMovement() {
	for (unsigned int i = 0; i < units.size(); i++) {
		units.at(i).UpdateMovement();
	}
}

std::vector<class Unit>* Player::getUnitsPtr() {
	return &units;
}

std::vector<std::vector<Vision::VisionStatus>> * Player::getVisionArrayPtr() {
	return vision.getVisionMapPtr();
}
