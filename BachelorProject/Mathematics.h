/*
Mathematics.h
Created by Martin Dariush Hansen, 2017-03-22
*/

#pragma once
class Mathematics
{
public:
	double CellDistance(int x0, int y0, int x1, int y1);
	double DistanceToCell(double unitX, double unitY, int cellX, int cellY);
	double DistanceToCellSquared(double unitX, double unitY, int cellX, int cellY);
};
