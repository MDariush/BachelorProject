/*
Pathfinder.h
Created by Martin Dariush Hansen, 2017-04-13
*/

#pragma once
#include "Mathematics.h"
#include <stack>

class Pathfinder
{
public:
	struct Node {
		std::vector<std::pair<double, std::pair<int, int>>> neighbors;
	};
	void Init(Map* pMapArg);
	std::stack<std::pair<double, double>> Pathfinder::AStar(double unitXArg, double unitYArg, double destXArg, double destYArg);
	std::vector<std::vector<Node>>* getNodesPtr();

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

	std::vector<std::vector<Node>> nodes;
	std::vector<std::vector<VisitedNode>> visitedNodes;
	std::vector<std::vector<bool>> exploredNodesMap;

	Mathematics math;
	class Map* pMap;
	int mapWidth;
	int mapHeight;
};
