/*
Player.cpp
Created by Martin Dariush Hansen, 2017-03-22
*/

#include "Player.h"
#include "Unit.h"

Player::Player() {
}

Player::~Player() {
}

void Player::Init(signed int mapWidth0, signed int mapHeight0) {
	vision.Init(&unitsPtr, mapWidth0, mapHeight0);
}

void Player::Step() {
	vision.FullUpdate();
}

void Player::AddUnit(Unit* unit0) {
	unitsPtr.push_back(unit0);
}

std::vector<std::vector<Vision::VisionStatus>> * Player::getVisionArrayPtr() {
	return vision.getVisionMapPtr();
}


