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
	void UpdateVisionForUnit(Unit* unitArg, std::vector<std::vector<VisionStatus>> visionMapTempArg);
	//void PointVision(int x, int y);
	//void LineVision(long double unitX, long double unitY, int xSpd, int ySpd, long double rng);
	void GenerateVisionForCell(long double unitX, long double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);
	//void FillVision(long double unitX, long double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);
	bool IsLegalCell(int x, int y);
	bool Vision::CanSeeCellEasily(int unitX, int unitY, int originX, int originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);
	bool Vision::CanSeeCellPrecisely(long double unitX, long double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);

	void setVisionMapSize(signed int width0, signed int height0);
	std::vector<std::vector<VisionStatus>>* getVisionMapPtr();
	void ResetVisionMap(unsigned int widthArg, unsigned int heightArg, VisionStatus statusArg);

private:
	std::vector<std::vector<VisionStatus>> visionMap;
	std::vector<std::vector<VisionStatus>> visionMapTemp;
	std::vector<std::vector<Map::CellStatus>>* pCellStatusArray;
};
