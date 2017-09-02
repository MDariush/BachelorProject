/*
Vision.h
Created by Martin Dariush Hansen, 2017-03-22
*/

#pragma once
//#include "Mathematics.h"
#include <vector>

class Vision
{
public:
	enum VisionStatus { UNEXPLORED, EXPLORED, VISIBLE };

	Vision();
	~Vision();
	void Init(class Map* pMapArg, class Pathfinder* pPathfinderArg);
	void FullUpdate(std::vector<class Unit>* pUnitsArg);
	void UpdateVisionForUnit(Unit* unitArg, std::vector<std::vector<VisionStatus>> visionMapTempArg);
	void GenerateVisionForCell(double unitX, double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);
	void ApplyVisionForCell(int xArg, int yArg, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);
	bool Vision::CanSeeCellEasily(int unitX, int unitY, int originX, int originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);
	bool Vision::CanSeeCellPrecisely(double unitX, double unitY, double originX, double originY, std::vector<std::vector<VisionStatus>>* visionMapTempPtrArg);

	class Map* getMapPtr();
	std::vector<std::vector<VisionStatus>>* getVisionMapPtr();
	int * getGenerationPtr();
	void ResetVisionMap(int widthArg, int heightArg, VisionStatus statusArg);

private:
	std::vector<std::vector<VisionStatus>> visionMap;
	std::vector<std::vector<VisionStatus>> visionMapPrevious;
	std::vector<std::vector<VisionStatus>> visionMapCalculations;
//	Mathematics math;
	class Map* pActualMap;
	class Map map;
	class Pathfinder* pPathfinder;
	int generation;
	bool generationIncreased;
};
