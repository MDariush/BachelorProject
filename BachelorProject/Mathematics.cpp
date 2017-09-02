/*
Mathematics.cpp
Created by Martin Dariush Hansen, 2017-04-21
*/

#include "Mathematics.h"
#include <math.h>

Mathematics* Mathematics::pInstance = NULL;

Mathematics* Mathematics::Instance() {
	if (!pInstance) {
		pInstance = new Mathematics;
	}
	return pInstance;
}

double Mathematics::CellDistanceEuclidean(int x0, int y0, int x1, int y1) {
	unsigned int xOffset = AbsoluteValue(x1 - x0);
	unsigned int yOffset = AbsoluteValue(y1 - y0);

	if (x0 == x1) {
		return yOffset;
	}
	else if (x1 == y1) {
		return xOffset;
	}

	// Resize array if coordinates are out of range
	if (xOffset > rangeArray.size() || yOffset > rangeArray.size()) {
		unsigned int highestOffset = xOffset;
		if (yOffset > highestOffset) {
			highestOffset = yOffset;
		}
		unsigned int previouslyHighestOffset = rangeArray.size();

		rangeArray.resize(highestOffset);
		for (int i = 0; i < highestOffset; i++) {
			rangeArray.at(i).resize(highestOffset);
		}

		// Calculate new distance values
		for (unsigned int xOffset = 1; xOffset <= highestOffset; xOffset++) {
			for (unsigned int yOffset = previouslyHighestOffset + 1; yOffset <= highestOffset; yOffset++) {
				rangeArray[xOffset - 1][yOffset - 1] = sqrt((xOffset * xOffset) + (yOffset * yOffset));
			}
		}
		for (unsigned int xOffset = previouslyHighestOffset + 1; xOffset <= highestOffset; xOffset++) {
			for (unsigned int yOffset = 1; yOffset <= previouslyHighestOffset; yOffset++) {
				rangeArray[xOffset - 1][yOffset - 1] = sqrt((xOffset * xOffset) + (yOffset * yOffset));
			}
		}
	}

	return rangeArray[xOffset - 1][yOffset - 1];
}

double Mathematics::CellDistanceManhattan(int x0, int y0, int x1, int y1) {
	unsigned int xOffset = AbsoluteValue(x1 - x0);
	unsigned int yOffset = AbsoluteValue(y1 - y0);

	return xOffset + yOffset;
}

unsigned int Mathematics::AbsoluteValue(int signedArg) {
	if (signedArg >= 0) {
		return signedArg;
	}
	return -signedArg;
}

double Mathematics::CellDistance(int x0, int y0, int x1, int y1) {
	return (double)sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

double Mathematics::DistanceToCell(double unitX, double unitY, int cellX, int cellY) {
	return (double)sqrt((cellX + 0.5 - unitX) * (cellX + 0.5 - unitX) + (cellY + 0.5 - unitY) * (cellY + 0.5 - unitY));
}

double Mathematics::DistanceToCellSquared(double unitX, double unitY, int cellX, int cellY) {
	return (cellX + 0.5 - unitX) * (cellX + 0.5 - unitX) + (cellY + 0.5 - unitY) * (cellY + 0.5 - unitY);
}
