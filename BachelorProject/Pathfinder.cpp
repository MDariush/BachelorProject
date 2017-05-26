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
	switch (GRAPH_TYPE) {
	case VISIBILITY_DECOMPOSED:
		visibilitySectionWidth = VISIBILITY_SECTION_WIDTH;
		visibilitySectionHeight = VISIBILITY_SECTION_HEIGHT;
		visibilityXSections = ceil(mapWidth / (double)visibilitySectionWidth);
		visibilityYSections = ceil(mapHeight / (double)visibilitySectionHeight);
		CreateVisibilityGraph();
		break;
	case VISIBILITY_FULL:
		visibilitySectionWidth = mapWidth;
		visibilitySectionHeight = mapHeight;
		visibilityXSections = 1;
		visibilityYSections = 1;
		CreateVisibilityGraph();
		break;
	default:
		CreateGridGraph();
		break;
	}

	ResetExploration();
}

void Pathfinder::UpdateGraph(int xArg, int yArg) {
	switch (GRAPH_TYPE) {
	case VISIBILITY_DECOMPOSED:
		visibilitySectionNodes.resize(visibilitySectionWidth, vector<std::set<std::pair<int, int>>>(visibilitySectionHeight));
		for (int i = 0; i < visibilitySectionWidth; i++) {
			for (int j = 0; j < visibilitySectionHeight; j++) {
				visibilitySectionNodes[i][j].clear();
			}
		}
		UpdateVisibilitySection(xArg, yArg);
		if (xArg != 0 && yArg != 0 && xArg % VISIBILITY_SECTION_WIDTH == 0 && yArg % VISIBILITY_SECTION_HEIGHT == 0) {
			UpdateVisibilitySection(xArg - 1, yArg);
			UpdateVisibilitySection(xArg, yArg - 1);
			UpdateVisibilitySection(xArg - 1, yArg - 1);
		}
		else if (xArg != 0 && xArg % VISIBILITY_SECTION_WIDTH == 0) {
			UpdateVisibilitySection(xArg - 1, yArg);
		}
		else if (yArg != 0 && yArg % VISIBILITY_SECTION_HEIGHT == 0) {
			UpdateVisibilitySection(xArg, yArg - 1);
		}
		break;
	case VISIBILITY_FULL:
		UpdateVisibilitySection(0, 0);
		break;
	default:
		UpdateGridGraphNode(xArg, yArg);
		break;
	}
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

	cout << "Generating A* path from " << startNode.x << ", " << startNode.y << " to " << destCellX << ", " << destCellY << ". Euclidean distance: " << startNode.totalCost << endl;

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
	nodes.resize(mapWidth, vector<Node>(mapHeight));

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

void Pathfinder::CreateVisibilityGraph() {

	cout << "Creating visibility graph for every section." << endl;

	nodes.resize(mapWidth, vector<Node>(mapHeight));
	visibilityNodes.resize(mapWidth, vector<bool>(mapHeight, false));

	for (int i = 0; i < visibilityXSections; i++) {
		for (int j = 0; j < visibilityYSections; j++) {
			UpdateGraph(i * visibilitySectionWidth + 1, j * visibilitySectionHeight + 1);
		}
	}
}

void Pathfinder::UpdateVisibilitySection(int xArg, int yArg) {

	int xSection = xArg / visibilitySectionWidth;
	int ySection = yArg / visibilitySectionHeight;

	cout << "Updating visibility graph at (" << xArg << ", " << yArg << "), section (" << xSection << ", " << ySection << ")." << endl;

	/*// Reset neighbor list of all nodes in section
	visibilitySectionNodes.resize(pMap->getWidth(), vector<std::set<std::pair<int, int>>>(pMap->getHeight()));
	for (int i = 0; i < mapWidth; i++) {
		for (int j = 0; j < mapHeight; j++) {
			visibilitySectionNodes[i][j].clear();
		}
	}*/

	// Clear visibility nodes in section
	visibilitySectionNodes[xSection][ySection].clear();
	for (int i = xSection * visibilitySectionWidth; i < (xSection + 1) * visibilitySectionWidth; i++) {
		for (int j = ySection * visibilitySectionHeight; j < (ySection + 1) * visibilitySectionHeight; j++) {
			if (pMap->IsLegalCell(i, j)) {
				visibilityNodes[i][j] = false;

				// Remove all edges in section going to nodes in same section
				if (IsWallNode(i, j)) {
					// @TODO: Optimize: Remove elements from the old list rather than filling up a new list
					forward_list<pair<double, pair<int, int>>> edgesToOtherSections;
					for (forward_list<pair<double, pair<int, int>>>::iterator it = nodes[i][j].neighbors.begin(); it != nodes[i][j].neighbors.end(); ) {
						if (it->second.first / visibilitySectionWidth != xSection || it->second.second / visibilitySectionHeight != ySection) {
							edgesToOtherSections.insert_after(edgesToOtherSections.before_begin(), *it);
						}
						++it;
					}
					nodes[i][j].neighbors = edgesToOtherSections;
				}
				else {
					nodes[i][j].neighbors.clear();
				}
			}
		}
	}

	// Create visibility nodes in section
	for (int i = xSection * visibilitySectionWidth; i < (xSection + 1) * visibilitySectionWidth; i++) {
		for (int j = ySection * visibilitySectionHeight; j < (ySection + 1) * visibilitySectionHeight; j++) {
			if (pMap->IsLegalCell(i, j)) {
				CreateVisibilityNode(xSection, ySection, i, j);
			}
		}
	}

	// Create visibility edges for cells in section
	if (GRAPH_TYPE == VISIBILITY_DECOMPOSED) {
		for (int i = xSection * visibilitySectionWidth; i <= (xSection + 1) * visibilitySectionWidth; i++) {
			for (int j = ySection * visibilitySectionHeight; j <= (ySection + 1) * visibilitySectionHeight; j++) {
				if (pMap->IsLegalCell(i, j)) {
					CreateVisibilityEdges(xSection, ySection, i, j);
				}
			}
		}
	}
}

void Pathfinder::CreateVisibilityNode(int xSectionArg, int ySectionArg, int xArg, int yArg) {
	if (IsAtCorner(xArg, yArg)) {
		//cout << "Creating visibility graph node at (" << xArg << ", " << yArg << ")." << endl;
		/*set<Node*>::iterator it = visibilityNodesPtr.at(visibilitySectionX).at(visibilitySectionY).find(&visibilityGridNodes[xArg][yArg]);
		if (it == visibilityNodesPtr.at(visibilitySectionX).at(visibilitySectionY).end()) {
		}*/
		visibilitySectionNodes.at(xSectionArg).at(ySectionArg).insert(std::make_pair(xArg, yArg));
		visibilityNodes[xArg][yArg] = true;
	}
	else {
		visibilityNodes[xArg][yArg] = false;
	}

	generation++;
}

bool Pathfinder::IsAtCorner(int xArg, int yArg) {
	// #TODO: Avoid recomputation? - store in 2d array?
	if (pMap->IsLegalCell(xArg, yArg) && pMap->getCellStatus(xArg, yArg) == pMap->OPEN) {
		for (int a = -1; a <= 1; a += 2) {
			for (int b = -1; b <= 1; b += 2) {
				if (pMap->IsLegalCell(xArg + a, yArg + b)
					&& pMap->getCellStatus(xArg + a, yArg + b) == pMap->CLOSED
					&& pMap->getCellStatus(xArg + a, yArg) == pMap->OPEN
					&& pMap->getCellStatus(xArg, yArg + b) == pMap->OPEN
					&& ((!pMap->IsLegalCell(xArg + a * 2, yArg) || pMap->getCellStatus(xArg + a * 2, yArg) == pMap->OPEN)
						|| (!pMap->IsLegalCell(xArg, yArg + b * 2) || pMap->getCellStatus(xArg, yArg + b * 2) == pMap->OPEN))) {

					return true;
				}
			}
		}
	}
	return false;
}

void Pathfinder::CreateVisibilityEdges(int xSectionArg, int ySectionArg, int xArg, int yArg) {

	double edgeWeight;

	// Find reachable visibility nodes in section
	/*//set<pair<int, int>>::iterator it = visibilitySectionNodes.at(xSectionArg).at(ySectionArg).find(std::make_pair(xArg, yArg));
	set<pair<int, int>>::iterator it = visibilitySectionNodes.at(xSectionArg).at(ySectionArg).begin();
	while (it != visibilitySectionNodes.at(xSectionArg).at(ySectionArg).end()) {
		//cout << "Looping through visibility node " << it->first << ", " << it->second << endl;
		if ((it->first != xArg || it->second != yArg)
			&& StraightLineIsOpen(xArg, yArg, it->first, it->second)) {
			
			edgeWeight = math.CellDistance(xArg, yArg, it->first, it->second);

			// Add nodes to cell
			nodes[xArg][yArg].neighbors.insert_after(nodes[xArg][yArg].neighbors.before_begin(), make_pair(edgeWeight, make_pair(it->first, it->second)));

			// Add cell to node's neighbor list if cell is in visibility graph
			if (visibilityNodes[xArg][yArg]) {
				nodes[it->first][it->second].neighbors.insert_after(nodes[it->first][it->second].neighbors.before_begin(), make_pair(edgeWeight, make_pair(xArg, yArg)));
			}

			//cout << "Edge created between visibility graph nodes (" << xArg << ", " << yArg << ") and (" << it->first << ", " << it->second << ")." << endl;
		}
		++it;
	}*/

	// Find reachable visibility graph section wall nodes and create 2-way edges
	//int sectionX = xSectionArg * visibilitySectionWidth;
	//int sectionY = ySectionArg * visibilitySectionHeight;
	int xMin = xSectionArg * visibilitySectionWidth;
	int yMin = ySectionArg * visibilitySectionHeight;
	int xMax = xMin + visibilitySectionWidth;
	int yMax = yMin + visibilitySectionHeight;
	if (xMax >= mapWidth) {
		xMax = mapWidth - 1;
	}
	if (yMax >= mapHeight) {
		yMax = mapHeight - 1;
	}
	for (int i = xMin; i <= xMax; i++) {
		CreateWallEdge(xArg, yArg, i, yMin);
		CreateWallEdge(xArg, yArg, i, yMax);
	}
	for (int i = yMin + 1; i < yMax; i++) {
		CreateWallEdge(xArg, yArg, xMin, i);
		CreateWallEdge(xArg, yArg, xMax, i);
	}

	/*for (int i = 0; i <= visibilitySectionWidth; i++) {
		CreateWallEdge(xArg, yArg, sectionX + i, sectionY);
		CreateWallEdge(xArg, yArg, sectionX + i, sectionY + visibilitySectionHeight);
	}
	for (int i = 1; i < visibilitySectionHeight; i++) {
		CreateWallEdge(xArg, yArg, sectionX, sectionY + i);
		CreateWallEdge(xArg, yArg, sectionX + visibilitySectionWidth, sectionY + i);
	}*/

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

void Pathfinder::CreateWallEdge(int x0Arg, int y0Arg, int x1Arg, int y1Arg) {

	/*if (x1Arg == mapWidth && y1Arg != 0 && y1Arg != mapHeight) {
		x1Arg -= 1;
	}
	if (y1Arg == mapHeight && x1Arg != 0 && x1Arg != mapWidth) {
		y1Arg -= 1;
	}*/

	if (pMap->IsLegalCell(x0Arg, y0Arg)
		&& pMap->IsLegalCell(x1Arg, y1Arg)
		//&& pMap->getCellStatus(x0Arg, y0Arg) == pMap->OPEN
		//&& pMap->getCellStatus(x1Arg, y1Arg) == pMap->OPEN
		&& (x0Arg != x1Arg || y0Arg != y1Arg)
		/*&& ((x1Arg != 0 && x1Arg != mapWidth - 1) || (y0Arg != 0 && y0Arg != mapHeight - 1 && y1Arg % sectionHeightArg == 0))
		&& ((y1Arg != 0 && y1Arg != mapHeight - 1) || (x0Arg != 0 && x0Arg != mapWidth - 1 && x1Arg % sectionWidthArg == 0))
		&& StraightLineIsOpen(x0Arg, y0Arg, x1Arg, y1Arg)*/) {

		int edgeWeight = math.CellDistance(x0Arg, y0Arg, x1Arg, y1Arg);
		nodes[x0Arg][y0Arg].neighbors.insert_after(nodes[x0Arg][y0Arg].neighbors.before_begin(), make_pair(edgeWeight, make_pair(x1Arg, y1Arg)));
		if (visibilityNodes[x0Arg][y0Arg] || IsWallNode(x0Arg, y0Arg)) {
			nodes[x1Arg][y1Arg].neighbors.insert_after(nodes[x1Arg][y1Arg].neighbors.before_begin(), make_pair(edgeWeight, make_pair(x0Arg, y0Arg)));
		}
		//cout << "Edge created between cell (" << x0Arg << ", " << y0Arg << ") and section wall (" << x1Arg << ", " << y1Arg << ")." << endl;
	}
}

bool Pathfinder::IsWallNode(int xArg, int yArg) {
	return xArg == 0
		|| yArg == 0
		|| xArg == mapWidth - 1
		|| yArg == mapHeight - 1
		|| xArg % visibilitySectionWidth == 0
		|| yArg % visibilitySectionHeight == 0;
}

bool Pathfinder::StraightLineIsOpen(int x0Arg, int y0Arg, int x1Arg, int y1Arg) {

	//cout << x0Arg << ", " << y0Arg << ", " << x1Arg << ", " << y1Arg << endl;

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
	double xSpd = 0.0;
	double ySpd = 0.0;
	
	if (xDistance > 0) {
		xSpd = 1.0;
	}
	else if (xDistance < 0) {
		xSpd = -1.0;
	}
	if (yDistance > 0) {
		ySpd = 1.0;
	}
	else if (yDistance < 0) {
		ySpd = -1.0;
	}

	double xLoopSpd = xSpd;
	double yLoopSpd = ySpd;

	if (absXDistance > absYDistance) {
		if (absYDistance == 0) {
			xSpd = xDistance;
		}
		else {
			xSpd = xDistance / (double)absYDistance;
		}
	}
	else {
		if (absXDistance == 0) {
			ySpd = yDistance;
		}
		else {
			ySpd = yDistance / (double)absXDistance;
		}
	}

	// Check collision - Horizontally
	double x = (double)x0Arg + 0.5;
	double y = (double)y0Arg + 0.5;

	if (ySpd == 0.0) {
		while ((int)x != x1Arg) {
			if (pMap->getCellStatus(x, y) == Map::CLOSED) {
				//cout << "CLOSED " << x << ", " << y << endl;
				return false;
			}
			//cout << " OPEN " << x << ", " << y << endl;
			x += xLoopSpd;
		}
	}

	// @TODO: Merge the collision checking into one method
	// Check collision - Vertically
	else if (xSpd == 0.0) {
		while ((int)y != y1Arg) {
			if (pMap->getCellStatus(x, y) == Map::CLOSED) {
				//cout << "CLOSED " << x << ", " << y << endl;
				return false;
			}
			//cout << " OPEN " << x << ", " << y << endl;
			y += yLoopSpd;
		}
	}

	// Check collision - Mostly horizontally
	else if (absXDistance > absYDistance) {
		double xStart = x - xSpd - xLoopSpd * 0.49999;;
		double xEnd = x + xSpd + xLoopSpd * 0.49999;;
		bool xEndReached = false;
		bool yEndReached = false;
		while (!yEndReached) {
			xEndReached = false;
			while (!xEndReached) {
				if (pMap->getCellStatus(x, y) == Map::CLOSED) {
					//cout << "CLOSED " << x << ", " << y << endl;
					return false;
				}
				//cout << " OPEN " << x << ", " << y << endl;
				if (x == xEnd || (int)x == x1Arg) {
					xStart += xSpd;
					xEnd += xSpd;
					x = xStart;
					xEndReached = true;
				}
				else {
					bool xIsGreaterThanXEnd = x > xEnd;
					x += xLoopSpd;
					if (xIsGreaterThanXEnd != x > xEnd) {
						x = xEnd;
					}
				}
			}
			if ((int)y == y1Arg) {
				yEndReached = true;
			}
			else {
				y += yLoopSpd;
			}
		}
	}

	// Check collision - Mostly vertically
	else {
		double yStart = y - ySpd - yLoopSpd * 0.49999;
		double yEnd = y + ySpd + yLoopSpd * 0.49999;;
		bool xEndReached = false;
		bool yEndReached = false;
		while (!xEndReached) {
			yEndReached = false;
			while (!yEndReached) {
				if (pMap->getCellStatus(x, y) == Map::CLOSED) {
					//cout << "CLOSED " << x << ", " << y << endl;
					return false;
				}
				//cout << " OPEN " << x << ", " << y << endl;
				if (y == yEnd || (int)y == y1Arg) {
					yStart += ySpd;
					yEnd += ySpd;
					y = yStart;
					yEndReached = true;
				}
				else {
					bool yIsGreaterThanYEnd = y > yEnd;
					y += yLoopSpd;
					if (yIsGreaterThanYEnd != y > yEnd) {
						y = yEnd;
					}
				}
			}
			if ((int)x == x1Arg) {
				xEndReached = true;
			}
			else {
				x += xLoopSpd;
			}
		}
	}
	return true;
}

// @TODO: Unused?
bool Pathfinder::VisibilityNodeInSection(int xArg, int yArg, int xSectionArg, int ySectionArg) {
	set<pair<int, int>>::iterator it = visibilitySectionNodes.at(xSectionArg).at(ySectionArg).find(std::make_pair(xArg, yArg));
	return it != visibilitySectionNodes.at(xSectionArg).at(ySectionArg).end();
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
