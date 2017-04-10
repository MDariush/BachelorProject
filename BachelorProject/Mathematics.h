/*
Mathematics.h
Created by Martin Dariush Hansen, 2017-03-22
*/

#pragma once
#include <math.h>

double DistanceToCell(long double unitX, long double unitY, int cellX, int cellY) {
	return sqrt((cellX + 0.5 - unitX) * (cellX + 0.5 - unitX) + (cellY + 0.5 - unitY) * (cellY + 0.5 - unitY));
}

double DistanceToCellSquared(long double unitX, long double unitY, int cellX, int cellY) {
	return (cellX + 0.5 - unitX) * (cellX + 0.5 - unitX) + (cellY + 0.5 - unitY) * (cellY + 0.5 - unitY);
}
