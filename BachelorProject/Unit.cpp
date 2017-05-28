/*
Unit.cpp
Created by Martin Dariush Hansen, 2017-03-17
*/

#include "Configurations.h"
#include "Constants.h"
#include "Map.h"
#include "Pathfinder.h"
#include "Unit.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
using namespace std;

Unit::Unit() {
}

Unit::~Unit() {
}

void Unit::Init(int playerArg, double xArg, double yArg, class Map* pMapArg, class Pathfinder* pPathfinderArg) {
	pMap = pMapArg;
	pPathfinder = pPathfinderArg;
	player = playerArg;
	x = xArg;
	y = yArg;

	commandIssued = false;
	moving = false;
	pathGeneration = -1;

	// #TODO
	direction = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / PI_X2));
	orientation = 0.0;
	orientationAcc = (PI_X2 / STEPS_PER_SECOND) / 0.5;
	spdMax = 2.0 / STEPS_PER_SECOND;
	spdAcc = (spdMax / STEPS_PER_SECOND) / 1.0;
	spdBrk = (spdMax / STEPS_PER_SECOND) / 1.0;
	spd = 0;
	setVisionRng(4.0 + 0.5);
	hpMax = 100;
	hp = hpMax;

	// Queue initial commands
	IssueCommand(MOVE, x, y);

	for (int i = 0; i < UNIT_DESTINATIONS; i++) {

		double randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0)) + 0.5;
		double randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0)) + 0.5;

		while (pMap->getCellStatus(randomX, randomY) != Map::OPEN) {
			randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0)) + 0.5;
			randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0)) + 0.5;
		}
		IssueCommand(MOVE, randomX, randomY);
	}
	IssueCommand(SUICIDE, 0.0, 0.0);
}

void Unit::Step() {
	ProcessCommands();
	Act();
}

void Unit::IssueCommand(Unit::CommandType commandTypeArg, double xArg, double yArg) {
	commandQueue.push(command());
	commandQueue.back().commandType = commandTypeArg;
	commandQueue.back().x = xArg;
	commandQueue.back().y = yArg;
}

void Unit::ProcessCommands() {
	if (commandQueue.size() > 0) {
		switch (commandQueue.front().commandType) {
		case MOVE:
			if (commandIssued) {
				if (IsInSquare(commandQueue.front().x, commandQueue.front().y, spd * 2) || commandQueue.front().path.size() == 0) {
					commandIssued = false;
					moving = false;
					commandQueue.pop();
					cout << "Move command completed" << endl;
				}
				else {
					if (IsInSquare(commandQueue.front().path.top().first, commandQueue.front().path.top().second, spd * 2)) {
						commandQueue.front().path.pop();
					}

					// Update path if there are spotted changes to the terrain
					if (pPathfinder->getGeneration() != pathGeneration) {
						cout << "Updating path" << endl;
						commandQueue.front().path = GeneratePath(x, y, commandQueue.front().x, commandQueue.front().y);
						pathGeneration = pPathfinder->getGeneration();
					}
				}
			}
			else {
				commandQueue.front().path = GeneratePath(x, y, commandQueue.front().x, commandQueue.front().y);
				pathGeneration = pPathfinder->getGeneration();
				commandIssued = true;
				moving = true;
			}
			break;

		case SUICIDE:
			hp = 0;
			break;
		default:
			break;
		}
	}
}

void Unit::Act() {

	// Set direction towards target
	if (moving && commandQueue.front().path.size() != 0) {
		//direction = atan2(y - commandQueue.front().y, x - commandQueue.front().x) - PI;
		direction = atan2(y - commandQueue.front().path.top().second, x - commandQueue.front().path.top().first) - PI;
	}

	// Set direction between -180 and 180 deg
	if (orientation != direction) {
		while (direction < -PI) {
			direction += PI_X2;
		}
		while (direction > PI) {
			direction -= PI_X2;
		}
		while (orientation < -PI) {
			orientation += PI_X2;
		}
		while (orientation > PI) {
			orientation -= PI_X2;
		}

		// Slowly rotate orientation towards direction
		if (!((orientation > direction - orientationAcc / 2 && orientation < direction + orientationAcc / 2)
			|| (orientation > direction - orientationAcc / 2 + PI_X2 && orientation < direction + orientationAcc / 2 + PI_X2)
			|| (orientation > direction - orientationAcc / 2 - PI_X2 && orientation < direction + orientationAcc / 2 - PI_X2))) {

			if (fmod(orientation - direction + PI_X2, PI_X2) - PI > 0.0) {
				orientation += orientationAcc;
			}
			else {
				orientation -= orientationAcc;
			}
		}
	}
	
	// Accelerate
	if (moving) {
		if (spd < spdMax) {
			spd += spdAcc;
			if (spd > spdMax) {
				spd = spdMax;
			}
		}
	}

	// Break
	else {
		if(spd > 0) {
			spd -= spdBrk;
			if (spd < 0) {
				spd = 0;
			}
		}
	}
	
	// Move
	if (spd != 0) {
		x += spd * cos(direction);
		y += spd * sin(direction);
	}

	// Die if outside map
	if (x < 0.0
		|| y < 0.0
		|| x >= pMap->getWidth()
		|| y >= pMap->getHeight()) {

		hp = 0;
	}
}

stack<std::pair<double, double>> Unit::GeneratePath(double unitXArg, double unitYArg, double destXArg, double destYArg) {
	return pPathfinder->AStar(unitXArg, unitYArg, destXArg, destYArg);
}

bool Unit::IsInSquare(double xArg, double yArg, double diameterArg) {
	return x >= xArg - diameterArg / 2
		&& x < xArg + diameterArg / 2
		&& y >= yArg - diameterArg / 2
		&& y < yArg + diameterArg / 2;
}

bool Unit::IsInRect(double x1Arg, double x2Arg, double y1Arg, double y2Arg) {
	return x >= x1Arg && x < x2Arg && y >= y1Arg && y < y2Arg;
}

double Unit::getX() {
	return x;
}

double Unit::getY() {
	return y;
}

double Unit::getOrientation() {
	return orientation;
}

double Unit::getOrientationDeg() {
	return orientation * PI_X2_TO_DEG;
}

double Unit::getVisionRng() {
	return visionRng;
}

double Unit::getVisionRngSquared() {
	return visionRngSquared;
}

Unit::command Unit::getCurrentCommand() {
	return commandQueue.front();
}

int Unit::getHp() {
	return hp;
}

void Unit::setVisionRng(double visionRngArg) {
	visionRng = visionRngArg;
	visionRngSquared = visionRng * visionRng;
}
