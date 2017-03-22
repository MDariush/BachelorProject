/*
Vision.cpp
Created by Martin Dariush Hansen, 2017-03-22
*/

#include "Vision.h"
#include <iostream>
using namespace std;

Vision::Vision() {
}

Vision::~Vision() {
}

void Vision::Init(std::vector<class Unit*>* unitsPtr0, signed int width0, signed int height0) {
	unitsPtr = unitsPtr0;
	UpdateVisionMapSize(width0, height0);
}

void Vision::FullUpdate() {

}

void Vision::UpdateVisionMapSize(signed int width0, signed int height0) {
	visionMap.resize(width0);
	for (int i = 0; i < width0; i++) {
		visionMap[i].resize(height0);
	}
	cout << "Vision map created with size " << visionMap.size() << "x" << visionMap.at(0).size() << ".\n";
}
