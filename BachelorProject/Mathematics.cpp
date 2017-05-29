/*
Mathematics.cpp
Created by Martin Dariush Hansen, 2017-04-21
*/

#include "Mathematics.h"
#include <math.h>

double Mathematics::CellDistance(int x0, int y0, int x1, int y1) {
	return (double)sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

double Mathematics::DistanceToCell(double unitX, double unitY, int cellX, int cellY) {
	return (double)sqrt((cellX + 0.5 - unitX) * (cellX + 0.5 - unitX) + (cellY + 0.5 - unitY) * (cellY + 0.5 - unitY));
}

double Mathematics::DistanceToCellSquared(double unitX, double unitY, int cellX, int cellY) {
	return (cellX + 0.5 - unitX) * (cellX + 0.5 - unitX) + (cellY + 0.5 - unitY) * (cellY + 0.5 - unitY);
}
