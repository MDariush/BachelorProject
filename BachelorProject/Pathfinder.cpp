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
	ResetClosedSet();
}

std::queue<pair<double, double>> Pathfinder::AStar(double unitXArg, double unitYArg, double destXArg, double destYArg) {
	queue<pair<double, double>> path;
	
	if (closedSet.size() != pMap->getWidth() || closedSet.at(0).size() != pMap->getHeight()) {
		ResetClosedSet();
	}
	else {
		for (int i = 0; i < pMap->getWidth(); i++) {
			for (int j = 0; j < pMap->getHeight(); j++) {
				closedSet.at(i).at(j).cost = -1;
			}
		}
	}
	
	priority_queue<ExploredNode, std::vector<ExploredNode>, CompareCost> openSet;

	// Add start node to open set
	ExploredNode startNode;
	startNode.cost = 0.0;
	startNode.x = unitXArg;
	startNode.y = unitYArg;
	startNode.cameFromX = startNode.x;
	startNode.cameFromY = startNode.y;
	openSet.push(startNode);

	// While goal not reached
	while (!openSet.empty()) {

		// Visit best nearby node
		closedSet.at(openSet.top().x).at(openSet.top().y).cost += openSet.top().cost;
		closedSet.at(openSet.top().x).at(openSet.top().y).cameFromX = openSet.top().cameFromX;
		closedSet.at(openSet.top().x).at(openSet.top().y).cameFromY = openSet.top().cameFromY;
		openSet.pop();

		// Reconstruct path if goal reached
		if (openSet.top().x == destXArg && openSet.top().y) {

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



void Pathfinder::ResetClosedSet() {
	VisitedNode initialNodes;
	initialNodes.cost = -1.0;
	initialNodes.cameFromX = -1;
	initialNodes.cameFromY = -1;

	closedSet.resize(pMap->getWidth(), vector<VisitedNode>(pMap->getHeight(), initialNodes));
}

void Pathfinder::ReconstructPath() {
	
}
