/*
Mathematics.h
Created by Martin Dariush Hansen, 2017-03-22
*/

#pragma once
#include <vector>

class Mathematics
{
public:
	static Mathematics* Instance();

	double CellDistanceEuclidean(int x0, int y0, int x1, int y1);
	double CellDistanceManhattan(int x0, int y0, int x1, int y1);
	unsigned int AbsoluteValue(int signedArg);

	// #TODO: Delete functions?
	double CellDistance(int x0, int y0, int x1, int y1);
	double DistanceToCell(double unitX, double unitY, int cellX, int cellY);
	double DistanceToCellSquared(double unitX, double unitY, int cellX, int cellY);
private:
	Mathematics() {};
	Mathematics(Mathematics const&) {};
	Mathematics& operator=(Mathematics const&) {};
	static Mathematics* pInstance;

	std::vector<std::vector<double>> rangeArray;
};
