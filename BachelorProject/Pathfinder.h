/*
Pathfinder.h
Created by Martin Dariush Hansen, 2017-04-13
*/

#pragma once
#include "Mathematics.h"
#include <forward_list>
#include <stack>

class Pathfinder
{
public:
	struct Node {
		std::forward_list<std::pair<double, std::pair<int, int>>> neighbors;
	};
	void Init(class Vision* pVisionArg);
	void UpdateGridGraphNode(int xArg, int yArg);
	std::stack<std::pair<double, double>> Pathfinder::AStar(double unitXArg, double unitYArg, double destXArg, double destYArg);
	std::vector<std::vector<Node>>* getNodesPtr();
	bool getGraphUpdated();
	void setGraphUpdated(bool graphUpdatedArg);

private:
	struct ExploredNode {
		double costFromStart;
		double totalCost;
		int x, y, cameFromX, cameFromY;
	};

	struct VisitedNode {
		double cost;
		int cameFromX, cameFromY;
	};

	struct CompareCost {
		constexpr bool operator()(ExploredNode const & a, ExploredNode const & b) const noexcept {
			return a.totalCost > b.totalCost;
		}
	};

	void ResetExploration();
	void CreateGridGraph();
	void CreateVisibilityGraph();

	bool IsAtCorner(int xArg, int yArg, int xOffsetArg, int yOffsetArg);

	std::vector<std::vector<Node>> nodes;
	std::vector<std::vector<VisitedNode>> visitedNodes;
	std::vector<std::vector<bool>> exploredNodesMap;

	Mathematics math;
	class Map* pMap;
	class Vision* pVision;
	int mapWidth;
	int mapHeight;
	bool graphUpdated;
};
