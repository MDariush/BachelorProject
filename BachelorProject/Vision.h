/*
Vision.h
Created by Martin Dariush Hansen, 2017-03-22
*/

#pragma once
#include <vector>

class Vision
{
public:
	enum VisionStatus { UNEXPLORED, EXPLORED, VISIBLE };

	Vision();
	~Vision();
	void Init(std::vector<class Unit*>* unitsPtr0, signed int width0, signed int height0);
	void FullUpdate();
	void UpdateVisionForUnit(Unit* unit0);
	
	void setVisionMapSize(signed int width0, signed int height0);
	std::vector<std::vector<VisionStatus>>* getVisionMapPtr();
	void setEntireVisionMap(VisionStatus);

private:
	std::vector<class Unit*>* unitsPtr;
	std::vector<std::vector<VisionStatus>> visionMap;
};
