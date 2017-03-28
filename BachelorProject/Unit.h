/*
Unit.h
Created by Martin Dariush Hansen, 2017-03-17
*/

#pragma once
#include <queue>



class Unit
{
public:
	enum CommandType {
		MOVE
	};
	struct command {
		CommandType commandType;
		long double x;
		long double y;
	};
	Unit();
	~Unit();
	void Init(signed int player0, long double x0, long double y0);
	void UpdateMovement();
	long double getX();
	long double getY();
	long double getOrientation();
	long double getOrientationDeg();
	long double getVisionRng();
	long double getVisionRngSquared();
	void setVisionRng(long double visionRng0);

private:
	signed int player;
	long double x, y;
	long double direction;
	long double orientation;
	long double orientationAcc;
	long double spd;
	long double spdAcc;
	long double spdMax;
	long double visionRng;
	long double visionRngSquared;
	std::queue<command> commandQueue;
};
