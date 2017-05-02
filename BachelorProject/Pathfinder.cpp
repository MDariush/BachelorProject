/*
Pathfinder.cpp
Created by Martin Dariush Hansen, 2017-04-13
*/

#include "Constants.h"
#include "Map.h"
#include "Pathfinder.h"
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

void Pathfinder::Init(class Map* pMapArg) {
	// #TODO: Include vision
	pMap = pMapArg;
	mapWidth = pMap->getWidth();
	mapHeight = pMap->getHeight();

	CreateGridGraph();
	ResetExploration();
}

std::stack<pair<double, double>> Pathfinder::AStar(double unitXArg, double unitYArg, double destXArg, double destYArg) {
	ResetExploration();
	std::priority_queue<ExploredNode, std::vector<ExploredNode>, CompareCost> exploredNodesQueue;
	stack<pair<double, double>> path;
	int destCellX = destXArg;
	int destCellY = destYArg;

	// Add start node to explored nodes
	ExploredNode startNode;
	startNode.x = unitXArg;
	startNode.y = unitYArg;
	startNode.cameFromX = startNode.x;
	startNode.cameFromY = startNode.y;
	startNode.costFromStart = 0.0;
	startNode.totalCost = math.CellDistance(startNode.x, startNode.y, destCellX, destCellY);
	exploredNodesQueue.push(startNode);
	exploredNodesMap[startNode.x][startNode.y] = true;

	//cout << "A* from " << startNode.x << ", " << startNode.y << " to " << destCellX << ", " << destCellY << ". Euclidean distance: " << startNode.totalCost << endl;

	// While there are still explored nodes
	while (!exploredNodesQueue.empty()) {

		// Visit cheapest explored node
		int currentX = exploredNodesQueue.top().x;
		int currentY = exploredNodesQueue.top().y;
		visitedNodes[currentX][currentY].cost = exploredNodesQueue.top().costFromStart;
		visitedNodes[currentX][currentY].cameFromX = exploredNodesQueue.top().cameFromX;
		visitedNodes[currentX][currentY].cameFromY = exploredNodesQueue.top().cameFromY;
		exploredNodesQueue.pop();
		exploredNodesMap[currentX][currentY] = false;
		//cout << "Visited " << currentX << ", " << currentY << endl;

		// Explore nearby nodes
		if (currentX != destCellX || currentY != destCellY) {
			for (int i = 0; i < nodes[currentX][currentY].neighbors.size(); i++) {

				int neighborX = nodes[currentX][currentY].neighbors[i].second.first;
				int neighborY = nodes[currentX][currentY].neighbors[i].second.second;

				if (visitedNodes[neighborX][neighborY].cost == -1.0
					&& exploredNodesMap[neighborX][neighborY] == false) {

					ExploredNode exploredNode;
					exploredNode.x = neighborX;
					exploredNode.y = neighborY;
					exploredNode.cameFromX = currentX;
					exploredNode.cameFromY = currentY;
					exploredNode.costFromStart = visitedNodes[currentX][currentY].cost + nodes[currentX][currentY].neighbors[i].first;
					exploredNode.totalCost = exploredNode.costFromStart + math.CellDistance(exploredNode.x, exploredNode.y, destCellX, destCellY);
					exploredNodesQueue.push(exploredNode);
					exploredNodesMap[neighborX][neighborY] = true;
					//cout << "Explored " << neighborX << ", " << neighborY << endl;
				}
			}
		}

		// Reconstruct path if goal reached
		else {
			int unitCellX = startNode.x;
			int unitCellY = startNode.y;
			int backtrackCellX = destCellX;
			int backtrackCellY = destCellY;

			//cout << "Reconstructing path from destination " << destXArg << ", " << destYArg << " to unit " << unitXArg << ", " << unitYArg << endl;

			path.push(make_pair(destXArg, destYArg));
			int backtrackCellXPrev = backtrackCellX;

			while (backtrackCellX != unitCellX || backtrackCellY != unitCellY) {
				if (backtrackCellX == visitedNodes[backtrackCellX][backtrackCellY].cameFromX || backtrackCellY == visitedNodes[backtrackCellX][backtrackCellY].cameFromY) {
					cout << "OK BACKTRACKING" << endl;
				}
				else {
					cout << "ILLEGAL BACKTRACKING" << endl;
				}

				cout << backtrackCellX << ", " << backtrackCellY << " came from ";
				backtrackCellXPrev = backtrackCellX;
				backtrackCellX = visitedNodes[backtrackCellX][backtrackCellY].cameFromX;
				backtrackCellY = visitedNodes[backtrackCellXPrev][backtrackCellY].cameFromY;
				cout << backtrackCellX << ", " << backtrackCellY << endl;
				path.push(make_pair(backtrackCellX + 0.5, backtrackCellY + 0.5));
			}

			// Remove the cell that the unit is inside from the path
			if (path.size() > 1) {
				path.pop();
			}
			return path;
		}
	}

	return path;
}

std::vector<std::vector<Pathfinder::Node>>* Pathfinder::getNodesPtr() {
	return &nodes;
}

void Pathfinder::CreateGridGraph() {
	nodes.resize(pMap->getWidth(), vector<Node>(pMap->getHeight()));

	// For every node in the grid
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(0).size(); j++) {
			if (pMap->getCellStatus(i, j) == pMap->OPEN) {

				// For all its neighbors
				for (int a = -1; a <= 1; a++) {
					for (int b = -1; b <= 1; b++) {

						if (pMap->IsLegalCell(i + a, j + b)
							&& pMap->getCellStatus(i + a, j + b) == pMap->OPEN
							&& (a != 0 || b != 0)) {

							if (a == 0 || b == 0) {
								nodes[i][j].neighbors.push_back(make_pair(1.0, make_pair(i + a, j + b)));
							}
							else if (pMap->getCellStatus(i + a, j) == pMap->OPEN
								&& pMap->getCellStatus(i, j + b) == pMap->OPEN) {

								nodes[i][j].neighbors.push_back(make_pair(HYPOTENUSE_SCALAR, make_pair(i + a, j + b)));
								cout << i + a << ", " << j + b << endl;
							}
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(0).size(); j++) {
			if (pMap->getCellStatus(i, j) == pMap->OPEN) {
				//cout << "Created node at " << i << ", " << j << " with neighbors: " << endl;
				for (int k = 0; k < nodes[i][j].neighbors.size(); k++) {
					//cout << nodes[i][j].neighbors[k].second.first << ", " << nodes[i][j].neighbors[k].second.second << endl;
				}
			}
		}
	}
}

void Pathfinder::ResetExploration() {
	if (visitedNodes.size() != pMap->getWidth() || visitedNodes.at(0).size() != pMap->getHeight()) {
		VisitedNode initialNodes;
		initialNodes.cost = -1.0;
		initialNodes.cameFromX = -1;
		initialNodes.cameFromY = -1;
		visitedNodes.resize(pMap->getWidth(), vector<VisitedNode>(pMap->getHeight(), initialNodes));
		exploredNodesMap.resize(pMap->getWidth(), vector<bool>(pMap->getHeight(), false));
	}
	else {
		for (int i = 0; i < pMap->getWidth(); i++) {
			for (int j = 0; j < pMap->getHeight(); j++) {
				visitedNodes[i][j].cost = -1.0;
				exploredNodesMap[i][j] = false;
			}
		}
	}
}
