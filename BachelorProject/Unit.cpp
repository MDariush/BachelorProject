/*
Unit.cpp
Created by Martin Dariush Hansen, 2017-03-17
*/

#include "Configurations.h"
#include "Constants.h"
#include "Map.h"
#include "Unit.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;

Unit::Unit() {
}

Unit::~Unit() {
}

void Unit::Init(int playerArg, double xArg, double yArg, class Map* pMapArg) {
	pMap = pMapArg;
	player = playerArg;
	x = xArg;
	y = yArg;
	commandIssued = false;
	moving = false;

	// #TODO
	direction = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / PI_X2));
	orientation = 0.0;
	orientationAcc = (PI_X2 / STEPS_PER_SECOND) / 0.5;
	spdMax = 6.0 / STEPS_PER_SECOND;
	spdAcc = (spdMax / STEPS_PER_SECOND) / 1.0;
	spdBrk = (spdMax / STEPS_PER_SECOND) / 1.0;
	spd = 0;
	setVisionRng(8.0 + 0.5);
	hpMax = 100;
	hp = hpMax;

	// Queue move commands and suicide command
	for (int i = 0; i < UNIT_DESTINATIONS; i++) {

		double randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
		double randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));

		while (pMap->getCellStatus(randomX, randomY) != Map::OPEN) {
			randomX = floor((pMap->getWidth() * rand()) / (RAND_MAX + 1.0));
			randomY = floor((pMap->getHeight() * rand()) / (RAND_MAX + 1.0));
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
				if (IsInRect(commandQueue.front().x - spdMax / 2,
					commandQueue.front().x + spdMax / 2,
					commandQueue.front().y - spdMax / 2,
					commandQueue.front().y + spdMax / 2)) {

					commandIssued = false;
					moving = false;
					commandQueue.pop();
				}
			}
			else {
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
	if (moving) {
		direction = atan2(y - commandQueue.front().y, x - commandQueue.front().x) - PI;
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
}

bool Unit::IsInRect(double x1, double x2, double y1, double y2) {
	return (x >= x1 && x < x2 && y >= y1 && y < y2);
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

int Unit::getHp() {
	return hp;
}

void Unit::setVisionRng(double visionRngArg) {
	visionRng = visionRngArg;
	visionRngSquared = visionRng * visionRng;
}
