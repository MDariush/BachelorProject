/*
Vision.cpp
Created by Martin Dariush Hansen, 2017-03-22
*/

#include "Constants.h"
#include "Mathematics.h"
#include "Unit.h"
#include "Vision.h"
#include <iostream>
using namespace std;

Vision::Vision() {
}

Vision::~Vision() {
}

void Vision::Init(std::vector<std::vector<Map::CellStatus>>* pCellStatusArrayArg) {
	pCellStatusArray = pCellStatusArrayArg;

	ResetVisionMap(pCellStatusArray->size(), pCellStatusArray->at(0).size(), VisionStatus::UNEXPLORED);
}

void Vision::FullUpdate(std::vector<class Unit>* pUnitsArg) {
	for (int i = 0; i < pCellStatusArray->size(); i++) {
		for (int j = 0; j < pCellStatusArray->at(0).size(); j++) {
			if (visionMap[i][j] == VisionStatus::VISIBLE)
			visionMap[i][j] = VisionStatus::EXPLORED;
		}
	}
	
	for (int u = 0; u < pUnitsArg->size(); u++) {
		UpdateVisionForUnit(&pUnitsArg->at(u));
	}
}

void Vision::UpdateVisionForUnit(Unit* unit0) {
	long double visionRng = unit0->getVisionRng();

	if (visionRng > 0) {
		long double x = unit0->getX();
		long double y = unit0->getY();


		// Skip vision outside map
		int iMin = x - visionRng;
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
		}

		// Fill the field of view
		// Vision on unit position
		visionMap.at(x).at(y) = VisionStatus::VISIBLE;

		// Line vision for every 45 deg
		LineVision(x + 1, y, 1, 0, visionRng);
		/*lineVision(x + 1, y + 1, 1, 1, visionRng * HYPOTENUSE_SCALAR);
		lineVision(x, y + 1, 0, 1, visionRng);
		lineVision(x - 1, y + 1, -1, 1, visionRng * HYPOTENUSE_SCALAR);
		lineVision(x - 1, y, -1, 0, visionRng);
		lineVision(x + 1, y + 1, 1, 1, visionRng * HYPOTENUSE_SCALAR);
		lineVision(x, y - 1, 0, -1, visionRng);
		lineVision(x + 1, y + 1, 1, 1, visionRng * HYPOTENUSE_SCALAR);*/

		// Fill the rest of the vision circle with 90 deg lines
		for (int i = 1; i < visionRng * HYPOTENUSE_SCALAR; i ++) {

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

		for (int i = 1; i <= unit0->getVisionRng() * HYPOTENUSE_SCALAR; i++) {

		}
		for (int i = iMin; i < iMax; i++) {
			for (int j = jMin; j < jMax; j++) {
				if (visionMap[i][j] != VisionStatus::VISIBLE
					&& distanceToCellSquared(unit0->getX(), unit0->getY(), i, j) <= unit0->getVisionRngSquared()) {
					visionMap[i][j] = VisionStatus::VISIBLE;
				}
			}
		}*/
	}
}

void Vision::LineVision(long double x, long double y, unsigned int xSpd, unsigned int ySpd, long double rng) {
	for (int i = 1; i <= rng; i++) {
		if (IsLegalCell(x + i * xSpd, y + i * ySpd)) {
			cout << mapWidth << " " << mapHeight << endl;
			if (pCellStatusArray->at(x + i * xSpd).at(y + i * ySpd) == Map::OPEN) {
				visionMap.at(x + i * xSpd).at(y + i * ySpd) = VisionStatus::VISIBLE;
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}
}

bool Vision::IsLegalCell(int x, int y) {
	return x >= 0 && x < mapWidth && y >= 0 && y < mapHeight;
}

void Vision::ResetVisionMap(unsigned int widthArg, unsigned int heightArg, VisionStatus statusArg) {
	visionMap.clear();
	visionMap.resize(widthArg, std::vector<VisionStatus>(heightArg, statusArg));
}

void Vision::setVisionMapSize(signed int width0, signed int height0) {
	visionMap.resize(width0);
	for (int i = 0; i < width0; i++) {
		visionMap[i].resize(height0);
	}
	cout << "Vision map created with size " << visionMap.size() << "x" << visionMap.at(0).size() << ".\n";
}

std::vector<std::vector<Vision::VisionStatus>>* Vision::getVisionMapPtr() {
	return &visionMap;
}
