/*
Unit.h
Created by Martin Dariush Hansen, 2017-03-17
*/

#pragma once
#include <stack>
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
		std::stack<std::pair<double, double>> path;
	};
	Unit();
	~Unit();
	void Init(int playerArg, double xArg, double yArg, class Map* pMapArg, class Pathfinder* pPathfinderArg);
	void Step();
	void IssueCommand(Unit::CommandType commandTypeArg, double xArg, double yArg);
	void ProcessCommands();
	void Act();
	bool IsInSquare(double xArg, double yArg, double diameterArg);
	bool IsInRect(double x1Arg, double x2Arg, double y1Arg, double y2Arg);
	double getX();
	double getY();
	double getOrientation();
	double getOrientationDeg();
	double getVisionRng();
	double getVisionRngSquared();
	command getCurrentCommand();
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
	int pathGeneration;
};
