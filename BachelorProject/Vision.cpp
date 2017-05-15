/*
Vision.cpp
Created by Martin Dariush Hansen, 2017-03-22
*/

#include "Constants.h"
#include "Map.h"
#include "Pathfinder.h"
#include "Unit.h"
#include "Vision.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

Vision::Vision() {
}

Vision::~Vision() {
}

void Vision::Init(class Map* pMapArg, class Pathfinder* pPathfinderArg) {
	pActualMap = pMapArg;
	map = *pMapArg;
	pPathfinder = pPathfinderArg;
	map.Clear(Map::OPEN);
	ResetVisionMap(pActualMap->getCellStatusArrayPtr()->size(), pActualMap->getCellStatusArrayPtr()->at(0).size(), VisionStatus::UNEXPLORED);
}

void Vision::FullUpdate(std::vector<class Unit>* pUnitsArg) {
	for (int i = 0; i < pActualMap->getCellStatusArrayPtr()->size(); i++) {
		for (int j = 0; j < pActualMap->getCellStatusArrayPtr()->at(0).size(); j++) {
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

		// Vision on unit position
		if (pActualMap->IsLegalCell(x, y)) {
			ApplyVisionForCell(x, y, &visionMapTempArg);
		}

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
				if (math.DistanceToCellSquared(x, y, floor(x) - i + j + 0.5, floor(y) - i + 0.5) <= unitArg->getVisionRngSquared()) {
					GenerateVisionForCell(x, y, floor(x) - i + j + 0.5, floor(y) - i + 0.5, &visionMapTempArg);
				}
				if (math.DistanceToCellSquared(x, y, floor(x) - i + j + 0.5, floor(y) + i + 0.5) <= unitArg->getVisionRngSquared()) {
					GenerateVisionForCell(x, y, floor(x) - i + j + 0.5, floor(y) + i + 0.5, &visionMapTempArg);
				}
				if (math.DistanceToCellSquared(x, y, floor(x) - i + 0.5, floor(y) - i + j + 0.5) <= unitArg->getVisionRngSquared()) {
					GenerateVisionForCell(x, y, floor(x) - i + 0.5, floor(y) - i + j + 0.5, &visionMapTempArg);
				}
				if (math.DistanceToCellSquared(x, y, floor(x) + i + 0.5, floor(y) - i + j + 0.5) <= unitArg->getVisionRngSquared()) {
					GenerateVisionForCell(x, y, floor(x) + i + 0.5, floor(y) - i + j + 0.5, &visionMapTempArg);
				}
			}
		}
	}
}

void Vision::GenerateVisionForCell(double unitX, double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg) {
	if (pActualMap->IsLegalCell(unitX, unitY) && pActualMap->IsLegalCell(originX, originY)) {
		if (CanSeeCellEasily(unitX, unitY, originX, originY, visionMapTempPtrArg)
			|| CanSeeCellPrecisely(unitX, unitY, originX, originY, visionMapTempPtrArg)) {

			ApplyVisionForCell(originX, originY, visionMapTempPtrArg);
		}
	}
}

void Vision::ApplyVisionForCell(int xArg, int yArg, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg) {
	visionMapTempPtrArg->at(xArg).at(yArg) = VisionStatus::VISIBLE;
	visionMap.at(xArg).at(yArg) = VisionStatus::VISIBLE;

	if (map.getCellStatus(xArg, yArg) != (pActualMap->getCellStatus(xArg, yArg))) {
		map.setCellStatus(pActualMap->getCellStatus(xArg, yArg), xArg, yArg);
		for (int a = -1; a <= 1; a++) {
			for (int b = -1; b <= 1; b++) {
				if (map.IsLegalCell(xArg + a, yArg + b)) {
					pPathfinder->UpdateGridGraphNode(xArg + a, yArg + b);
				}
			}
		}
	}
}

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
		&& pActualMap->getCellStatusArrayPtr()->at(originX + xSpd).at(originY + ySpd) == Map::OPEN) {

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
				&& pActualMap->getCellStatusArrayPtr()->at(originX + xSpd).at(originY + ySpd) == Map::OPEN;
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
		if (pActualMap->getCellStatusArrayPtr()->at(originX).at(originY) != Map::OPEN) {
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

Map* Vision::getMapPtr() {
	return &map;
}

std::vector<std::vector<Vision::VisionStatus>>* Vision::getVisionMapPtr() {
	return &visionMap;
}
