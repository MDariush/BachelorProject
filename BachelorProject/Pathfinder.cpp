/*
Pathfinder.cpp
Created by Martin Dariush Hansen, 2017-04-13
*/

#include "Configurations.h"
#include "Constants.h"
#include "Map.h"
#include "Pathfinder.h"
#include "Vision.h"
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

void Pathfinder::Init(Vision* pVisionArg) {
	pVision = pVisionArg;
	pMap = pVisionArg->getMapPtr();
	mapWidth = pMap->getWidth();
	mapHeight = pMap->getHeight();

	switch (GRAPH_TO_USE) {
	VISIBILITY_DECOMPOSED:
		CreateVisibilityGraph(VISIBILITY_SECTION_WIDTH, VISIBILITY_SECTION_HEIGHT);
	VISIBILITY_FULL:
		CreateVisibilityGraph(mapWidth, mapHeight);
	default:
		CreateGridGraph();
	}

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

	cout << "Generating A* grid graph path from " << startNode.x << ", " << startNode.y << " to " << destCellX << ", " << destCellY << ". Euclidean distance: " << startNode.totalCost << endl;

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
			for (forward_list<pair<double, pair<int, int>>>::iterator it = nodes[currentX][currentY].neighbors.begin(); it != nodes[currentX][currentY].neighbors.end(); ) {
			//for (int i = 0; i < nodes[currentX][currentY].neighbors.size(); i++) {

				int neighborX = it->second.first;
				int neighborY = it->second.second;

				if (visitedNodes[neighborX][neighborY].cost == -1.0
					&& exploredNodesMap[neighborX][neighborY] == false) {

					ExploredNode exploredNode;
					exploredNode.x = neighborX;
					exploredNode.y = neighborY;
					exploredNode.cameFromX = currentX;
					exploredNode.cameFromY = currentY;
					exploredNode.costFromStart = visitedNodes[currentX][currentY].cost + it->first;
					exploredNode.totalCost = exploredNode.costFromStart + math.CellDistance(exploredNode.x, exploredNode.y, destCellX, destCellY);
					exploredNodesQueue.push(exploredNode);
					exploredNodesMap[neighborX][neighborY] = true;
					//cout << "Explored " << neighborX << ", " << neighborY << endl;
				}

				++it;
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
				//cout << backtrackCellX << ", " << backtrackCellY << " came from ";
				backtrackCellXPrev = backtrackCellX;
				backtrackCellX = visitedNodes[backtrackCellX][backtrackCellY].cameFromX;
				backtrackCellY = visitedNodes[backtrackCellXPrev][backtrackCellY].cameFromY;
				//cout << backtrackCellX << ", " << backtrackCellY << endl;
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

	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(0).size(); j++) {
			UpdateGridGraphNode(i, j);
		}
	}
}

void Pathfinder::UpdateGridGraphNode(int xArg, int yArg) {
	nodes[xArg][yArg].neighbors.clear();

	// For all 8 cells around the cell
	for (int a = -1; a <= 1; a++) {
		for (int b = -1; b <= 1; b++) {

			// Ignore illegal neighbors
			if (pMap->IsLegalCell(xArg + a, yArg + b)
				&& (a != 0 || b != 0)) {

				// Set edge weight
				double edgeWeight = 1.0;
				if (a != 0 && b != 0) {
					edgeWeight = HYPOTENUSE_SCALAR;
				}

				// Remove cell from neighbor's neighbor list
				nodes[xArg + a][yArg + b].neighbors.remove(make_pair(edgeWeight, make_pair(xArg, yArg)));

				// If cell and neighbor is open and neighbor is horizontal/vertical or diagonal with open space in between
				if (pMap->getCellStatus(xArg, yArg) == pMap->OPEN
					&& pMap->getCellStatus(xArg + a, yArg + b) == pMap->OPEN
					&& pMap->getCellStatus(xArg + a, yArg) == pMap->OPEN
					&& pMap->getCellStatus(xArg, yArg + b) == pMap->OPEN) {

					// Add neighbor to cell
					nodes[xArg][yArg].neighbors.insert_after(nodes[xArg][yArg].neighbors.before_begin(), make_pair(edgeWeight, make_pair(xArg + a, yArg + b)));

					// Add cell to neighbor's neighbor list
					nodes[xArg + a][yArg + b].neighbors.insert_after(nodes[xArg + a][yArg + b].neighbors.before_begin(), make_pair(edgeWeight, make_pair(xArg, yArg)));
				}
			}
		}
	}

	generation++;
}

void Pathfinder::CreateVisibilityGraph(int sectionWidthArg, int sectionHeightArg) {
	visibilityGridNodes.resize(pMap->getWidth(), vector<Node>(pMap->getHeight()));

	for (int i = 0; i < ceil(visibilityGridNodes.size() / (double)sectionWidthArg); i++) {
		for (int j = 0; j < ceil(visibilityGridNodes.at(0).size() / (double)sectionHeightArg); j++) {
			UpdateVisibilitySection(i, j, sectionWidthArg, sectionHeightArg);
		}
	}
}

void Pathfinder::UpdateVisibilitySection(int xSectionArg, int ySectionArg, int sectionWidthArg, int sectionHeightArg) {
	visibilitySectionNodes.resize(pMap->getWidth(), vector<std::set<std::pair<int, int>>>(pMap->getHeight()));
	for (int i = xSectionArg * sectionWidthArg; i < (xSectionArg + 1) * sectionWidthArg; i++) {
		for (int j = ySectionArg * sectionHeightArg; j < (ySectionArg + 1) * sectionHeightArg; j++) {
			if (pMap->IsLegalCell(xSectionArg, ySectionArg)) {
				CreateVisibilityNode(xSectionArg, ySectionArg, i, j);
			}
		}
	}
	for (int i = xSectionArg * sectionWidthArg; i < (xSectionArg + 1) * sectionWidthArg; i++) {
		for (int j = ySectionArg * sectionHeightArg; j < (ySectionArg + 1) * sectionHeightArg; j++) {
			if (pMap->IsLegalCell(xSectionArg, ySectionArg)) {
				visibilityGridNodes[i][j].neighbors.clear();
				CreateVisibilityEdges(xSectionArg, ySectionArg, i, j, sectionWidthArg, sectionHeightArg);
			}
		}
	}
}

void Pathfinder::CreateVisibilityNode(int xSectionArg, int ySectionArg, int xArg, int yArg) {
	if (IsAtCorner(xArg, yArg)) {
		/*set<Node*>::iterator it = visibilityNodesPtr.at(visibilitySectionX).at(visibilitySectionY).find(&visibilityGridNodes[xArg][yArg]);
		if (it == visibilityNodesPtr.at(visibilitySectionX).at(visibilitySectionY).end()) {
		}*/
		visibilitySectionNodes.at(xSectionArg).at(ySectionArg).insert(std::make_pair(xArg, yArg));
	}

	generation++;
}

void Pathfinder::CreateVisibilityEdges(int xSectionArg, int ySectionArg, int xArg, int yArg, int sectionWidthArg, int sectionHeightArg) {

	double edgeWeight;

	// Find reachable visibility nodes in section
	set<pair<int, int>>::iterator it = visibilitySectionNodes.at(xSectionArg).at(ySectionArg).find(std::make_pair(xArg, yArg));
	while (it != visibilitySectionNodes.at(xSectionArg).at(ySectionArg).end()) {
		if (it->first != xArg && it->second != yArg && StraightLineIsOpen(xArg, yArg, it->first, it->second)) {
			
			edgeWeight = math.CellDistance(xArg, yArg, it->first, it->second);

			// Add nodes to cell
			visibilityGridNodes[xArg][yArg].neighbors.insert_after(visibilityGridNodes[xArg][yArg].neighbors.before_begin(), make_pair(edgeWeight, make_pair(it->first, it->second)));

			// Add cell to node's list if cell is in visibility graph
			set<pair<int, int>>::iterator it = visibilitySectionNodes.at(xSectionArg).at(ySectionArg).find(std::make_pair(xArg, yArg));
			if (it != visibilitySectionNodes.at(xSectionArg).at(ySectionArg).end()) {
				visibilityGridNodes[it->first][it->second].neighbors.insert_after(visibilityGridNodes[it->first][it->second].neighbors.before_begin(), make_pair(edgeWeight, make_pair(xArg, yArg)));
			}
		}

		++it;
	}

	// Find reachable visibility graph section wall nodes and create 2-way edges
	int sectionX = xSectionArg * sectionWidthArg;
	int sectionY = ySectionArg * sectionHeightArg;

	for (int i = 0; i < sectionWidthArg; i++) {
		if (StraightLineIsOpen(xArg, yArg, sectionX + i, sectionY)
			&& VisibilityNodeInSection(sectionX + i, sectionY, xSectionArg, ySectionArg)) {
			CreateBidirectedVisibilityEdge(xArg, yArg, sectionX + i, sectionY);
		}
		if (StraightLineIsOpen(xArg, yArg, sectionX + i, sectionY + sectionHeightArg)
			&& VisibilityNodeInSection(sectionX + i, sectionY + sectionHeightArg, xSectionArg, ySectionArg)) {
			CreateBidirectedVisibilityEdge(xArg, yArg, sectionX + i, sectionY + sectionHeightArg);
		}
	}
	for (int i = 0; i < sectionHeightArg; i++) {
		if (StraightLineIsOpen(xArg, yArg, sectionX, sectionY + i)
			&& VisibilityNodeInSection(sectionX, sectionY + i, xSectionArg, ySectionArg)) {
			CreateBidirectedVisibilityEdge(xArg, yArg, sectionX, sectionY + i);
		}
		if (StraightLineIsOpen(xArg, yArg, sectionX + sectionWidthArg, sectionY + i)
			&& VisibilityNodeInSection(sectionX + sectionWidthArg, sectionY + i, xSectionArg, ySectionArg)) {
			CreateBidirectedVisibilityEdge(xArg, yArg, sectionX + sectionWidthArg, sectionY + i);
		}
	}

	/*else {
		visibilitySectionNodes.at(xSectionArg).at(ySectionArg).erase(std::make_pair(xArg, yArg));
	}*/

		/*if (a == 0 || b == 0) {
		nodes[i][j].neighbors.push_back(make_pair(1.0, make_pair(i + a, j + b)));
		}
		else if (pMap->getCellStatus(i + a, j) == pMap->OPEN
		&& pMap->getCellStatus(i, j + b) == pMap->OPEN) {

		nodes[i][j].neighbors.push_back(make_pair(HYPOTENUSE_SCALAR, make_pair(i + a, j + b)));
		cout << i + a << ", " << j + b << endl;
		}

	}*/

	generation++;
}

bool Pathfinder::IsAtCorner(int xArg, int yArg) {
	if (pMap->IsLegalCell(xArg, yArg) || pMap->getCellStatus(xArg, yArg) == pMap->CLOSED) {
		return false;
	}
	for (int a = -1; a <= 1; a += 2) {
		for (int b = -1; b <= 1; b += 2) {
			if (pMap->IsLegalCell(xArg + a, yArg + b)
				&& pMap->getCellStatus(xArg + a, yArg + b) == pMap->CLOSED
				&& (pMap->getCellStatus(xArg + a * 2, yArg) == pMap->OPEN
					|| pMap->getCellStatus(xArg, yArg + b * 2) == pMap->OPEN)) {

				return true;
			}
		}
	}
	return false;
}

bool Pathfinder::StraightLineIsOpen(int x0Arg, int y0Arg, int x1Arg, int y1Arg) {

	// Calculate distances
	int xDistance = x1Arg - x0Arg;
	int yDistance = y1Arg - y0Arg;
	int absXDistance = xDistance;
	int absYDistance = yDistance;

	if (absXDistance < 0) {
		absXDistance = -absXDistance;
	}
	if (absYDistance < 0) {
		absYDistance = -absYDistance;
	}

	// Set the x and y speed
	double xSpd, ySpd = 1.0;
	int xLoopSpd, yLoopSpd = 1;

	if (xDistance < 0) {
		xSpd = -1.0;
		xLoopSpd = -1;
	}
	if (yDistance < 0) {
		ySpd = -1.0;
		yLoopSpd = -1;
	}

	if (absXDistance > absYDistance) {
		xSpd = xDistance / (double)absYDistance;
	}
	else {
		ySpd = yDistance / (double)absYDistance;
	}

	// Check collision with lines with a width of 2
	double x = x0Arg;
	double y = y0Arg;

	while ((int)x != x1Arg && (int)y != y1Arg) {
		for (int i = x; i < x + xSpd; i += xLoopSpd) {
			for (int j = y; j < y + ySpd; j += yLoopSpd) {
				if (pMap->getCellStatus(i, j) == Map::CLOSED) {
					return false;
				}
			}
		}
		x += xSpd;
		y += ySpd;
	}
	return true;
}

void Pathfinder::CreateBidirectedVisibilityEdge(int x0Arg, int y0Arg, int x1Arg, int y1Arg) {
	int edgeWeight = math.CellDistance(x0Arg, y0Arg, x1Arg, y1Arg);
	visibilityGridNodes[x0Arg][y0Arg].neighbors.insert_after(visibilityGridNodes[x0Arg][y0Arg].neighbors.before_begin(), make_pair(edgeWeight, make_pair(x1Arg, y1Arg)));
	visibilityGridNodes[x1Arg][y1Arg].neighbors.insert_after(visibilityGridNodes[x1Arg][y1Arg].neighbors.before_begin(), make_pair(edgeWeight, make_pair(x0Arg, y0Arg)));
}

bool Pathfinder::VisibilityNodeInSection(int xArg, int yArg, int xSectionArg, int ySectionArg) {
	set<pair<int, int>>::iterator it = visibilitySectionNodes.at(xSectionArg).at(ySectionArg).find(std::make_pair(xArg, yArg));
	return it == visibilitySectionNodes.at(xSectionArg).at(ySectionArg).end();
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

int Pathfinder::getGeneration() {
	return generation;
}

void Pathfinder::setGeneration(int generationArg) {
	generation = generationArg;
}
