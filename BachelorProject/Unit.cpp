/*
Unit.cpp
Created by Martin Dariush Hansen, 2017-03-17
*/

#include "Constants.h"
#include "Unit.h"
#include <stdlib.h>

Unit::Unit() {
}

Unit::~Unit() {
}

void Unit::Init(signed int player0, long double x0, long double y0) {
	player = player0;
	x = x0;
	y = y0;

	// #TODO
	direction = 0; // static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / PI_2));
	orientation = 0.0;
	orientationAcc = 0.125;
	spd = 2.0 / STEPS_PER_SECOND;
	spdAcc = 1.0 / 256.0;
	spdMax = 1.0 / 32.0;
	setVisionRng(4.0 + 0.5);
	commandQueue.push(command());
	commandQueue.front().commandType = MOVE;
	commandQueue.front().x = 0;
	commandQueue.front().y = 0;
}

void Unit::UpdateMovement() {

	if (commandQueue.size() > 0) {
		switch (commandQueue.front().commandType) {
		case MOVE:

			// #TODO Direction acc
			direction = atan2(commandQueue.front().x - x, y - commandQueue.front().y);

			break;
		default:
			break;
		}
	}

	// #TODO Orientation acc
	if (orientation != direction) {
		orientation = direction;
	}

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
	return orientation * PI_2_TO_DEG;
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
