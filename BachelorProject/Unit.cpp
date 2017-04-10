/*
Unit.cpp
Created by Martin Dariush Hansen, 2017-03-17
*/

#include "Constants.h"
#include "Unit.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;

Unit::Unit() {
}

Unit::~Unit() {
}

void Unit::Init(signed int player0, long double x0, long double y0) {
	player = player0;
	x = x0;
	y = y0;

	// #TODO
	direction = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / PI_X2));
	orientation = 0.0;
	orientationAcc = (PI_X2 / STEPS_PER_SECOND) / 0.5;
	spdMax = 1.0 / STEPS_PER_SECOND;
	spdAcc = (spdMax / STEPS_PER_SECOND) / 1.0;
	spdBrk = (spdMax / STEPS_PER_SECOND) / 1.0;
	spd = 0;
	setVisionRng(32.0 + 0.5);
	
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 9.5;
	commandQueue.back().y = 4.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 8.5;
	commandQueue.back().y = 3.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 7.5;
	commandQueue.back().y = 4.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 6.5;
	commandQueue.back().y = 3.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 5.5;
	commandQueue.back().y = 4.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 4.5;
	commandQueue.back().y = 4.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 4.5;
	commandQueue.back().y = 5.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 5.5;
	commandQueue.back().y = 5.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 5.5;
	commandQueue.back().y = 4.5;
	commandQueue.push(command());
	/*
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 20.5;
	commandQueue.back().y = 4.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 80.5;
	commandQueue.back().y = 9.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 60.5;
	commandQueue.back().y = 90.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 30.5;
	commandQueue.back().y = 70.5;
	commandQueue.push(command());
	commandQueue.back().commandType = MOVE;
	commandQueue.back().x = 10.5;
	commandQueue.back().y = 10.5;
	*/
}

void Unit::UpdateMovement() {


	if (commandQueue.size() > 0) {
		switch (commandQueue.front().commandType) {
		case MOVE:

			// Set direction towards target
			direction = atan2(y - commandQueue.front().y, x - commandQueue.front().x) - PI;

			// Accelerate
			if (spd < spdMax) {
				spd += spdAcc;
				if (spd > spdMax) {
					spd = spdMax;
				}
			}

			// Command completed
			if (x < commandQueue.front().x + spdMax / 2
				&& x > commandQueue.front().x - spdMax / 2
				&& y < commandQueue.front().y + spdMax / 2
				&& y > commandQueue.front().y - spdMax / 2) {
				commandQueue.pop();
			}

			break;
		default:
			break;
		}
	}

	// Set direction between -180 and 180 deg
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

		float rest = fmod(orientation - direction + PI_X2, PI_X2) - PI;
		if (rest > 0) {
			orientation += orientationAcc;
		}
		else {
			orientation -= orientationAcc;
		}
	}
	
	// Move
	if (spd != 0) {
		x += spd * cos(direction);
		y += spd * sin(direction);
	}
}

long double Unit::getX() {
	return x;
}

long double Unit::getY() {
	return y;
}

long double Unit::getOrientation() {
	return orientation;
}

long double Unit::getOrientationDeg() {
	return orientation * PI_X2_TO_DEG;
}

long double Unit::getVisionRng() {
	return visionRng;
}

long double Unit::getVisionRngSquared() {
	return visionRngSquared;
}

void Unit::setVisionRng(long double visionRng0) {
	visionRng = visionRng0;
	visionRngSquared = visionRng * visionRng;
}
