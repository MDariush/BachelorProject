/*
Unit.cpp
Created by Martin Dariush Hansen, 2017-03-17
*/

#include "Unit.h"

Unit::Unit() {
}

Unit::~Unit() {
}

void Unit::Init(signed int player0, long double x0, long double y0) {
	player = player0;
	x = x0;
	y = y0;
	direction = 0.0;
	orientation = 0.0;
	orientationAcc = 0.125;
	spdAcc = 1.0 / 256.0;
	maxSpd = 1.0 / 32.0;
	visionRng = 8;
}

long double Unit::getX() {
	return x;
}

long double Unit::getY() {
	return y;
}
