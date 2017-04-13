/*
Vision.cpp
Created by Martin Dariush Hansen, 2017-03-22
*/

#include "Constants.h"
#include "Mathematics.h"
#include "Unit.h"
#include "Vision.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

Vision::Vision() {
}

Vision::~Vision() {
}

void Vision::Init(class Map* pMapArg) {
	pMap = pMapArg;
	ResetVisionMap(pMap->getCellStatusArrayPtr()->size(), pMap->getCellStatusArrayPtr()->at(0).size(), VisionStatus::UNEXPLORED);
}

void Vision::FullUpdate(std::vector<class Unit>* pUnitsArg) {
	for (int i = 0; i < pMap->getCellStatusArrayPtr()->size(); i++) {
		for (int j = 0; j < pMap->getCellStatusArrayPtr()->at(0).size(); j++) {
			if (visionMap[i][j] == VisionStatus::VISIBLE)
			visionMap[i][j] = VisionStatus::EXPLORED;
		}
	}
	
	if (pUnitsArg->size() > 0) {
		visionMapTemp = visionMap;
		for (int u = 0; u < pUnitsArg->size(); u++) {
			UpdateVisionForUnit(&pUnitsArg->at(u), visionMapTemp);
		}
	}
}

void Vision::UpdateVisionForUnit(Unit* unitArg, std::vector<std::vector<VisionStatus>> visionMapTempArg) {
	double visionRng = unitArg->getVisionRng();
	
	if (visionRng > 0) {
		double x = unitArg->getX();
		double y = unitArg->getY();

		// Skip vision outside map
		/*int iMin = x - visionRng;
		int jMin = y - visionRng;
		int iMax = x + visionRng + 1.0;
		int jMax = y + visionRng + 1.0;
		if (iMin < 0) {
			iMin = 0;
		}
		if (jMin < 0) {
			jMin = 0;
		}
		if (iMax > visionMap.size()) {
			iMax = visionMap.size();
		}
		if (jMax > visionMap.at(0).size()) {
			jMax = visionMap.at(0).size();
		}*/

		// Vision on unit position
		if (pMap->IsLegalCell(x, y)) {
			visionMapTempArg.at(x).at(y) = VisionStatus::VISIBLE;
			visionMap.at(x).at(y) = VisionStatus::VISIBLE;
		}

		// Line vision for every 45 deg
		/*LineVision(x + 1, y, 1, 0, visionRng - 1);
		LineVision(x + 1, y + 1, 1, 1, (visionRng - 1) * HYPOTENUSE_AXIS_SCALAR);
		LineVision(x, y + 1, 0, 1, visionRng - 1);
		LineVision(x - 1, y + 1, -1, 1, (visionRng - 1) * HYPOTENUSE_AXIS_SCALAR);
		LineVision(x - 1, y, -1, 0, visionRng - 1);
		LineVision(x - 1, y - 1, -1, -1, (visionRng - 1) * HYPOTENUSE_AXIS_SCALAR);
		LineVision(x, y - 1, 0, -1, visionRng - 1);
		LineVision(x + 1, y - 1, 1, -1, (visionRng - 1) * HYPOTENUSE_AXIS_SCALAR);*/

		// Loop through 'inner box' in vision circle
		for (int i = 1; i < visionRng * HYPOTENUSE_AXIS_SCALAR; i++) {
			for (int j = 0; j < i * 2 + 1; j++) {
				GenerateVisionForCell(x, y, floor(x) - i + j + 0.5, floor(y) - i + 0.5, &visionMapTempArg);
				GenerateVisionForCell(x, y, floor(x) - i + j + 0.5, floor(y) + i + 0.5, &visionMapTempArg);
			}
			for (int j = 1; j < i * 2; j++) {
				GenerateVisionForCell(x, y, floor(x) - i + 0.5, floor(y) - i + j + 0.5, &visionMapTempArg);
				GenerateVisionForCell(x, y, floor(x) + i + 0.5, floor(y) - i + j + 0.5, &visionMapTempArg);
			}
		}

		// Loop through 'outer box' in vision circle
		for (int i = visionRng * HYPOTENUSE_AXIS_SCALAR + 1; i < visionRng; i++) {
			for (int j = 0; j < i * 2 + 1; j++) {
				if (DistanceToCellSquared(x, y, floor(x) - i + j + 0.5, floor(y) - i + 0.5) <= unitArg->getVisionRngSquared()) {
					GenerateVisionForCell(x, y, floor(x) - i + j + 0.5, floor(y) - i + 0.5, &visionMapTempArg);
				}
				if (DistanceToCellSquared(x, y, floor(x) - i + j + 0.5, floor(y) + i + 0.5) <= unitArg->getVisionRngSquared()) {
					GenerateVisionForCell(x, y, floor(x) - i + j + 0.5, floor(y) + i + 0.5, &visionMapTempArg);
				}
				if (DistanceToCellSquared(x, y, floor(x) - i + 0.5, floor(y) - i + j + 0.5) <= unitArg->getVisionRngSquared()) {
					GenerateVisionForCell(x, y, floor(x) - i + 0.5, floor(y) - i + j + 0.5, &visionMapTempArg);
				}
				if (DistanceToCellSquared(x, y, floor(x) + i + 0.5, floor(y) - i + j + 0.5) <= unitArg->getVisionRngSquared()) {
					GenerateVisionForCell(x, y, floor(x) + i + 0.5, floor(y) - i + j + 0.5, &visionMapTempArg);
				}
			}
		}


		/*for (int i = 1; i <= visionRng; i++) {
			if (pCellStatusArray->at(x + i).at(y) == Map::OPEN()) {
				visionMap.at(x + i).at(y) = VisionStatus::VISIBLE;
			}
			else {
				break;
			}
		}
		for (int i = 1; i <= visionRng; i++) {
			if (pCellStatusArray->at(x).at(y + i) == Map::OPEN()) {
				visionMap.at(x).at(y + i) = VisionStatus::VISIBLE;
			}
			else {
				break;
			}
		}
		for (int i = 1; i <= visionRng; i++) {
			if (pCellStatusArray->at(x - i).at(y) == Map::OPEN()) {
				visionMap.at(x - i).at(y) = VisionStatus::VISIBLE;
			}
			else {
				break;
			}
		}
		for (int i = 1; i <= visionRng; i++) {
			if (pCellStatusArray->at(x).at(y - i) == Map::OPEN()) {
				visionMap.at(x).at(y - i) = VisionStatus::VISIBLE;
			}
			else {
				break;
			}
		}

		for (int i = 1; i <= unit0->getVisionRng() * HYPOTENUSE_AXIS_SCALAR; i++) {

		}
		for (int i = iMin; i < iMax; i++) {
			for (int j = jMin; j < jMax; j++) {
				if (visionMap[i][j] != VisionStatus::VISIBLE
					&& DistanceToCellSquared(unit0->getX(), unit0->getY(), i, j) <= unit0->getVisionRngSquared()) {
					visionMap[i][j] = VisionStatus::VISIBLE;
				}
			}
		}*/
	}
}

/*void Vision::PointVision(int x, int y) {
	if (IsLegalCell(x, y)) {
		visionMap.at(x).at(y) = VisionStatus::VISIBLE;
	}
}*/

/*void Vision::LineVision(double unitX, double unitY, int xSpd, int ySpd, double rng) {
	for (int i = 0; i <= rng; i++) {
		if (IsLegalCell(unitX + i * xSpd, unitY + i * ySpd)) {
			visionMap.at(unitX + i * xSpd).at(unitY + i * ySpd) = VisionStatus::VISIBLE;
			if (pCellStatusArray->at(unitX + i * xSpd).at(unitY + i * ySpd) != Map::OPEN) {
				break;
			}
		}
		else {
			break;
		}
	}
}*/

void Vision::GenerateVisionForCell(double unitX, double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg) {
	if (pMap->IsLegalCell(unitX, unitY) && pMap->IsLegalCell(originX, originY)) {
		if (CanSeeCellEasily(unitX, unitY, originX, originY, visionMapTempPtrArg)
			|| CanSeeCellPrecisely(unitX, unitY, originX, originY, visionMapTempPtrArg)) {

			visionMapTempPtrArg->at(originX).at(originY) = VisionStatus::VISIBLE;
			visionMap.at(originX).at(originY) = VisionStatus::VISIBLE;
		}
	}
}

/*void Vision::FillVision(double unitX, double unitY, double originX, double originY, int xSpd, int ySpd, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg) {
	for (int i = 1; i < (originX - unitX) * xSpd + (originY - unitY) * ySpd; i++) {
		if (IsLegalCell(unitX + i * xSpd, unitY + i * ySpd)) {
			if (CanSeeCellEasily(unitX, unitY, originX, originY, visionMapTempPtrArg)
				|| CanSeeCellPrecisely(unitX, unitY, originX, originY, visionMapTempPtrArg)) {

				visionMapTempPtrArg->at(originX + xSpd).at(originY + ySpd) == VisionStatus::VISIBLE;
			}
		}
	}
}*/

bool Vision::CanSeeCellEasily(int unitX, int unitY, int originX, int originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg) {
	bool result = false;
	
	int xSpd = 0;
	int ySpd = 0;

	// Check a cell 1 step towards unit position
	if (unitX < originX) {
		xSpd = -1;
	}
	else if (unitX > originX) {
		xSpd = 1;
	}

	if (unitY < originY) {
		ySpd = -1;
	}
	else if (unitY > originY) {
		ySpd = 1;
	}

	if (visionMapTempPtrArg->at(originX + xSpd).at(originY + ySpd) == VisionStatus::VISIBLE
		&& pMap->getCellStatusArrayPtr()->at(originX + xSpd).at(originY + ySpd) == Map::OPEN) {

		result = true;

		// Check a cell next to origin and the other cell towards the goal
		if (unitX != originX
			&& unitY != originY
			&& (unitX - originX) * (unitX - originX) != (unitY - originY) * (unitY - originY)) {

			if ((unitX - originX) * (unitX - originX) < (unitY - originY) * (unitY - originY)) {
				xSpd = 0;
			}
			else {
				ySpd = 0;
			}

			result = visionMapTempPtrArg->at(originX + xSpd).at(originY + ySpd) == VisionStatus::VISIBLE
				&& pMap->getCellStatusArrayPtr()->at(originX + xSpd).at(originY + ySpd) == Map::OPEN;
		}
	}

	return result;
}

bool Vision::CanSeeCellPrecisely(double unitX, double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg) {
	
	// Calculate distances
	double xDistance = unitX - originX;
	double yDistance = unitY - originY;
	double absXDistance = xDistance;
	double absYDistance = yDistance;

	if (absXDistance < 0) {
		absXDistance = -absXDistance;
	}
	if (absYDistance < 0) {
		absYDistance = -absYDistance;
	}

	// Set the x and y speed
	double xSpd = 1.0;
	double ySpd = 1.0;

	if (xDistance < 0) {
		xSpd = -1.0;
	}
	if (yDistance < 0) {
		ySpd = -1.0;
	}

	if (absXDistance <= 1.0 && absYDistance <= 1.0) {
		return true;
	}
	else if (absXDistance <= 1.0) {
		xSpd = 0.0;
	}
	else if (absYDistance <= 1.0) {
		ySpd = 0.0;
	}
	else if (absXDistance > absYDistance) {
		ySpd = yDistance / absXDistance;
	}
	else {
		xSpd = xDistance / absYDistance;
	}

	// Move from origin to unit and check for collision
	int distanceAlongAxis = 0;

	if (xSpd == 1.0 || xSpd == -1.0) {
		distanceAlongAxis = absXDistance - 1;
	}
	else {
		distanceAlongAxis = absYDistance - 1;
	}
	for (int i = 0; i < distanceAlongAxis; i++) {
		originX += xSpd;
		originY += ySpd;
		if (pMap->getCellStatusArrayPtr()->at(originX).at(originY) != Map::OPEN) {
			return false;
		}
		/*if (visionMapTempPtrArg->at(originX).at(originY) == VisionStatus::VISIBLE) {
			return true;
		}*/
	}
	return true;
}

void Vision::ResetVisionMap(int widthArg, int heightArg, VisionStatus statusArg) {
	visionMap.clear();
	visionMap.resize(widthArg, std::vector<VisionStatus>(heightArg, statusArg));
}

void Vision::setVisionMapSize(int width0, int height0) {
	visionMap.resize(width0);
	for (int i = 0; i < width0; i++) {
		visionMap[i].resize(height0);
	}
	cout << "Vision map created with size " << visionMap.size() << "x" << visionMap.at(0).size() << ".\n";
}

std::vector<std::vector<Vision::VisionStatus>>* Vision::getVisionMapPtr() {
	return &visionMap;
}
