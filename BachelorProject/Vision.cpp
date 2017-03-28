/*
Vision.cpp
Created by Martin Dariush Hansen, 2017-03-22
*/

#include "Mathematics.h"
#include "Unit.h"
#include "Vision.h"
#include <iostream>
using namespace std;

Vision::Vision() {
}

Vision::~Vision() {
}

void Vision::Init(signed int widthArg, signed int heightArg) {
	setVisionMapSize(widthArg, heightArg);
	setEntireVisionMap(VisionStatus::UNEXPLORED);
}

void Vision::FullUpdate(std::vector<class Unit>* pUnitsArg) {
	for (int i = 0; i < visionMap.size(); i++) {
		for (int j = 0; j < visionMap.at(0).size(); j++) {
			if (visionMap[i][j] == VisionStatus::VISIBLE)
			visionMap[i][j] = VisionStatus::EXPLORED;
		}
	}
	
	for (int u = 0; u < pUnitsArg->size(); u++) {
		UpdateVisionForUnit(&pUnitsArg->at(u));
	}
}

void Vision::UpdateVisionForUnit(Unit* unit0) {

	// Skip vision outside map
	int iMin = unit0->getX() - unit0->getVisionRng();
	int jMin = unit0->getY() - unit0->getVisionRng();
	int iMax = unit0->getX() + unit0->getVisionRng() + 1.0;
	int jMax = unit0->getY() + unit0->getVisionRng() + 1.0;
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
	// #TODO Obstacles should block vision
	for (int i = iMin; i < iMax; i++) {
		for (int j = jMin; j < jMax; j++) {
			if (visionMap[i][j] != VisionStatus::VISIBLE 
				&& distanceToCellSquared(unit0->getX(), unit0->getY(), i, j) <= unit0->getVisionRngSquared()) {
				visionMap[i][j] = VisionStatus::VISIBLE;
			}
		}
	}
}

void Vision::setEntireVisionMap(VisionStatus status0) {
	for (int i = 0; i < visionMap.size(); i++) {
		for (int j = 0; j < visionMap.at(0).size(); j++) {
			visionMap[i][j] = status0;
		}
	}
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
