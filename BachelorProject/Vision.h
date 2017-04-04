/*
Vision.h
Created by Martin Dariush Hansen, 2017-03-22
*/

#pragma once
#include "Map.h"
#include <vector>

class Vision
{
public:
	enum VisionStatus { UNEXPLORED, EXPLORED, VISIBLE };

	Vision();
	~Vision();
	void Init(std::vector<std::vector<Map::CellStatus>>* pCellStatusArrayArg);
	void FullUpdate(std::vector<class Unit>* pUnitsArg);
	void UpdateVisionForUnit(Unit* unit0);
	void LineVision(long double x, long double y, unsigned int xSpd, unsigned int ySpd, long double visionRng);
	bool IsLegalCell(int x, int y);

	void setVisionMapSize(signed int width0, signed int height0);
	std::vector<std::vector<VisionStatus>>* getVisionMapPtr();
	void ResetVisionMap(unsigned int widthArg, unsigned int heightArg, VisionStatus statusArg);

private:
	std::vector<std::vector<VisionStatus>> visionMap;
	unsigned int mapWidth;
	unsigned int mapHeight;
	std::vector<std::vector<Map::CellStatus>>* pCellStatusArray;
};
