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
		MOVE,
		SUICIDE
	};
	struct command {
		CommandType commandType;
		double x;
		double y;
		std::queue<std::pair<double, double>> path;
	};
	Unit();
	~Unit();
	void Init(int playerArg, double xArg, double yArg, class Map* pMapArg, class Pathfinder* pPathfinderArg);
	void Step();
	void IssueCommand(Unit::CommandType commandTypeArg, double xArg, double yArg);
	void ProcessCommands();
	void Act();
	std::queue<std::pair<double, double>> Unit::GeneratePath(double unitXArg, double unitYArg, double destXArg, double destYArg);
	bool IsInRect(double x1, double x2, double y1, double y2);
	double getX();
	double getY();
	double getOrientation();
	double getOrientationDeg();
	double getVisionRng();
	double getVisionRngSquared();
	int getHp();
	void setVisionRng(double visionRngArg);

private:
	class Map* pMap;
	class Pathfinder* pPathfinder;

	int player;
	double x, y;
	double direction;
	double orientation;
	double orientationAcc;
	double spd;
	double spdAcc;
	double spdBrk;
	double spdMax;
	double visionRng;
	double visionRngSquared;
	int hp;
	int hpMax;
	std::queue<command> commandQueue;
	bool commandIssued;
	bool moving;
};
