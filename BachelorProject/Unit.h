/*
Unit.h
Created by Martin Dariush Hansen, 2017-03-17
*/

#pragma once

class Unit
{
public:
	Unit();
	~Unit();
	void Init(signed int player0, long double x0, long double y0);

	long double getX();
	long double getY();
	unsigned int getVisionRng();
	unsigned int getVisionRngSquared();
	void setVisionRng(signed int visionRng0);

private:
	signed int player;
	long double x, y;
	long double direction;
	long double orientation;
	long double orientationAcc;
	long double spdAcc;
	long double maxSpd;
	unsigned int visionRng;
	unsigned int visionRngSquared;
};
