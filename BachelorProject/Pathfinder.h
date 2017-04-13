/*
Pathfinder.h
Created by Martin Dariush Hansen, 2017-04-13
*/

#pragma once
#include <queue>

class Pathfinder
{
public:
	void Init(Map* pMapArg);
	std::queue<std::pair<double, double>> Pathfinder::AStar(double unitXArg, double unitYArg, double destXArg, double destYArg);
	void CreateGridGraph();
	void ResetClosedSet();
	void ReconstructPath();

private:
	struct Node {
		double cost;
		std::vector<std::pair<double, std::pair<int, int>>> neighbors;
	};

	struct ExploredNode {
		double cost;
		int x, y, cameFromX, cameFromY;
	};

	struct VisitedNode {
		double cost;
		int cameFromX, cameFromY;
	};

	struct CompareCost {
		constexpr bool operator()(ExploredNode const & a, ExploredNode const & b) const noexcept {
			return a.cost < b.cost;
		}
	};

	std::vector<std::vector<Node>> nodes;
	std::vector<std::vector<VisitedNode>> closedSet;

	class Map* pMap;
	int mapWidth;
	int mapHeight;
};


