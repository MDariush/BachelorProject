/*
Pathfinder.cpp
Created by Martin Dariush Hansen, 2017-04-13
*/

#include "Constants.h"
#include "Map.h"
#include "Pathfinder.h"
#include <vector>
using namespace std;

void Pathfinder::Init(class Map* pMapArg) {
	pMap = pMapArg;
	mapWidth = pMap->getWidth();
	mapHeight = pMap->getHeight();

	CreateGridGraph();
	ResetPath();
}

std::queue<pair<double, double>> Pathfinder::AStar(double unitXArg, double unitYArg, double destXArg, double destYArg) {
	queue<pair<double, double>> path;
	
	if (visitedNodes.size() != pMap->getWidth() || visitedNodes.at(0).size() != pMap->getHeight()) {
		ResetPath();
	}
	else {
		for (int i = 0; i < pMap->getWidth(); i++) {
			for (int j = 0; j < pMap->getHeight(); j++) {
				visitedNodes[i][j].cost = -1;
				exploredNodesMap[i][j] = false;
				
			}
		}
	}
	
	// Add start node to open set
	ExploredNode startNode;
	startNode.cost = 0.0;
	startNode.x = unitXArg;
	startNode.y = unitYArg;
	startNode.cameFromX = startNode.x;
	startNode.cameFromY = startNode.y;
	exploredNodesQueue.push(startNode);
	exploredNodesMap[unitXArg][unitYArg] = true;

	// While there are still nodes to visit
	while (!exploredNodesQueue.empty()) {

		// Visit best nearby node
		int currentX = exploredNodesQueue.top().x;
		int currentY = exploredNodesQueue.top().y;
		visitedNodes[currentX][currentY].cost += exploredNodesQueue.top().cost;
		visitedNodes[currentX][currentY].cameFromX = exploredNodesQueue.top().cameFromX;
		visitedNodes[currentX][currentY].cameFromY = exploredNodesQueue.top().cameFromY;
		exploredNodesQueue.pop();
		exploredNodesMap[currentX][currentY] = false;

		// Add nearby nodes to open set
		if (!(currentX == destXArg && currentY == destYArg)) {
			for (int i = 0; i < nodes[currentX][currentY].neighbors.size(); i++) {

				int neighborX = nodes[currentX][currentY].neighbors[i].second.first;
				int neighborY = nodes[currentX][currentY].neighbors[i].second.second;

				if (visitedNodes[neighborX][neighborY].cost == -1
					&& exploredNodesMap[neighborX][neighborY] == false) {

					ExploredNode exploredNode;
					exploredNode.x = neighborX;
					exploredNode.y = neighborY;
					exploredNode.cameFromX = currentX;
					exploredNode.cameFromY = currentY;
					exploredNode.cost =  nodes[currentX][currentY].neighbors[i].first;
					exploredNodesQueue.push(exploredNode);
				}
			}
		}

		// Reconstruct path if goal reached
		else {
			double backtrackNodeX = destXArg;
			double backtrackNodeY = destYArg;
			path.push(make_pair(destXArg, destYArg));
			
			while (true) {

				if (backtrackNodeX == unitXArg && backtrackNodeY == unitYArg) {

					break;
				}
			}
		}
	}

	return path;
}

void Pathfinder::CreateGridGraph() {
	nodes.resize(pMap->getWidth(), vector<Node>(pMap->getHeight()));

	// For every node
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.size(); j++) {

			// For all its neighbors
			for (int a = -1; a < 1; a++) {
				for (int b = -1; b < 1; b++) {

					if (pMap->IsLegalCell(i + a, j + a) 
						&& pMap->getCellStatus(i + b, j + b) == pMap->OPEN
						&& (i != 0 || j != 0)) {
						
						if (i == 0 || j == 0) {
							nodes.at(i).at(j).neighbors.push_back(make_pair(1.0, make_pair(i + 1, j + 1)));
						}
						else {
							nodes.at(i).at(j).neighbors.push_back(make_pair(HYPOTENUSE_SCALAR, make_pair(i + 1, j + 1)));
						}
					}
				}
			}
		}
	}
}

void Pathfinder::ResetPath() {
	VisitedNode initialNodes;
	initialNodes.cost = -1.0;
	initialNodes.cameFromX = -1;
	initialNodes.cameFromY = -1;
	visitedNodes.resize(pMap->getWidth(), vector<VisitedNode>(pMap->getHeight(), initialNodes));

	exploredNodesMap.resize(pMap->getWidth(), vector<bool>(pMap->getHeight(), false));

	exploredNodesQueue = priority_queue<ExploredNode, vector<ExploredNode>, CompareCost>();
}

void Pathfinder::ReconstructPath() {
	
}
