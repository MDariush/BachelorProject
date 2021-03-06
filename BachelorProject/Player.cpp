/*
Player.cpp
Created by Martin Dariush Hansen, 2017-03-22
*/

#include "Configurations.h"
#include "Player.h"
#include "Unit.h"
#include <iostream>
using namespace std;

Player::Player() {
}

Player::~Player() {
}

void Player::Init(int playerNumberArg, class Map* pMapArg, Timer* pTimerArg) {
	playerNumber = playerNumberArg;
	pTimer = pTimerArg;

	vision.Init(pMapArg, &pathfinder);
	pathfinder.Init(&vision, pTimer);
}

void Player::Step() {
	if (!IGNORE_VISION) {
		vision.FullUpdate(&units);
	}
}

void Player::CreateUnit(double xArg, double yArg, class Map* pMapArg) {
	units.push_back(Unit());
	units.at(units.size() - 1).Init(playerNumber, xArg, yArg, pMapArg, &pathfinder);
	//cout << "Unit created for Player " << playerNumber << " at (" << units.at(units.size() - 1).getX() << ", " << units.at(units.size() - 1).getY() << ").\n";
}

void Player::RemoveUnit(int indexArg) {
	//cout << "Unit removed for Player " << playerNumber << " at (" << units.at(indexArg).getX() << ", " << units.at(indexArg).getY() << ").\n";
	units.erase(units.begin() + indexArg);
}

void Player::UpdateUnits() {
	for (int i = 0; i < units.size(); i++) {
		units.at(i).Step();
	}
}

void Player::RemoveDeadUnits() {
	for (int i = 0; i < units.size(); i++) {
		if (units.at(i).getHp() <= 0) {
			RemoveUnit(i);
			i--;
		}
	}
}

std::vector<class Unit>* Player::getUnitsPtr() {
	return &units;
}

bool Player::HasUnitsInRect(double x0Arg, double y0Arg, double x1Arg, double y1Arg) {
	for (int i = 0; i < units.size(); i++) {
		if (units.at(i).getX() >= x0Arg
			&& units.at(i).getX() < x1Arg
			&& units.at(i).getY() >= y0Arg
			&& units.at(i).getY() < y1Arg) {
			return true;
		}
	}
	return false;
}

std::vector<std::vector<Vision::VisionStatus>> * Player::getVisionArrayPtr() {
	return vision.getVisionMapPtr();
}

class Pathfinder* Player::getPathfinderPtr() {
	return &pathfinder;
}

class Vision * Player::getVisionPtr() {
	return &vision;
}
