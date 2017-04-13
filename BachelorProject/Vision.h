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
	void Init(class Map* pMapArg);
	void FullUpdate(std::vector<class Unit>* pUnitsArg);
	void UpdateVisionForUnit(Unit* unitArg, std::vector<std::vector<VisionStatus>> visionMapTempArg);
	//void PointVision(int x, int y);
	//void LineVision(double unitX, double unitY, int xSpd, int ySpd, double rng);
	void GenerateVisionForCell(double unitX, double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);
	//void FillVision(double unitX, double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);
	bool Vision::CanSeeCellEasily(int unitX, int unitY, int originX, int originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);
	bool Vision::CanSeeCellPrecisely(double unitX, double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);

	void setVisionMapSize(int width0, int height0);
	std::vector<std::vector<VisionStatus>>* getVisionMapPtr();
	void ResetVisionMap(int widthArg, int heightArg, VisionStatus statusArg);

private:
	std::vector<std::vector<VisionStatus>> visionMap;
	std::vector<std::vector<VisionStatus>> visionMapTemp;
	class Map* pMap;
};
