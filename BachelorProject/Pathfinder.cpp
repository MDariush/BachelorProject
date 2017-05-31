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
	generation = 1;
}

void Pathfinder::UpdateGraph(int xArg, int yArg) {
	cout << "UpdateGraph xArg " << xArg << " yArg " << yArg << endl;

	// Define section and neighbor coordinates
	int xSection = xArg / visibilitySectionWidth;
	int ySection = yArg / visibilitySectionHeight;
	int xWallMin = xSection * visibilitySectionWidth;
	int yWallMin = ySection * visibilitySectionHeight;
	int xWallMax = xWallMin + visibilitySectionWidth;
	int yWallMax = yWallMin + visibilitySectionHeight;
	int xMin = xWallMin + 1;
	int yMin = yWallMin + 1;
	int xMax = xWallMax - 1;
	int yMax = yWallMax - 1;
	int neighborXMin = xWallMin - visibilitySectionWidth;
	if (neighborXMin < 0) {
		neighborXMin = 0;
	}
	int neighborYMin = yWallMin - visibilitySectionHeight;
	if (neighborYMin < 0) {
		neighborYMin = 0;
	}
	int neighborXMax = xWallMax + 1;
	if (neighborXMax >= mapWidth) {
		neighborXMax = mapWidth;
	}
	int neighborYMax = yWallMax + 1;
	if (neighborYMax >= mapHeight) {
		neighborYMax = mapHeight;
	}
	
	switch (GRAPH_TYPE) {
	case VISIBILITY_DECOMPOSED:
		ClearVisibilitySectionNodes(xMin, yMin, xMax, yMax, xSection, ySection);
		ClearSectionEdges(neighborXMin, neighborYMin, xWallMax, yWallMax, xWallMin, yWallMin, xMax, yMax);
		CreateVisibilitySectionNodes(xMin, yMin, xMax, yMax, xSection, ySection);
		CreateVisibilitySectionInternalEdges(xWallMin, yWallMin, xWallMax, yWallMax, xSection, ySection);
		CreateVisibilitySectionExternalEdges(neighborXMin, neighborYMin, xWallMin, yWallMin, xWallMax, yWallMax, xSection, ySection);

		/*ClearVisibilitySectionNodes(xMin, yMin, xMax, yMax, xSection, ySection);
		ClearOutboundEdgesInRect(xMin, yMin, xMax, yMax);
		ClearInnerWallEdges(xWallMin, yWallMin, xWallMax, yWallMax);
		CreateVisibilitySectionNodes(xMin, yMin, xMax, yMax, xSection, ySection);
		CreateVisibilitySectionEdges(xMin, yMin, xMax, yMax, xSection, ySection);
		CreateVisibilitySectionWallEdges(xMin, yMin, xMax, yMax, xWallMin, yWallMin, xWallMax, yWallMax);
		CreateEdgesFromOuterSectionsToInnerWall(xWallMin, yWallMin, xWallMax, yWallMax, neighborXMin, neighborYMin, neighborXMax, neighborYMax);*/
		break;
	case VISIBILITY_FULL:
		ClearVisibilitySectionNodes(0, 0, mapWidth - 1, mapHeight - 1, 0, 0);
		ClearOutboundEdgesInRect(xMin, yMin, xMax, yMax);

		CreateVisibilitySectionNodes(0, 0, mapWidth - 1, mapHeight - 1, 0, 0);

		/*ClearVisibilitySectionNodes(0, 0, mapWidth - 1, mapHeight - 1, 0, 0);
		ClearVisibilitySectionEdges(0, 0);
		CreateVisibilitySectionNodes(0, 0, mapWidth - 1, mapHeight - 1, 0, 0);
		CreateVisibilitySectionEdges(0, 0);*/
		break;
	default:
		UpdateGridGraphNode(xArg, yArg);
		break;
	}
}

stack<pair<double, double>> Pathfinder::GeneratePath(double unitXArg, double unitYArg, double destXArg, double destYArg) {
	cout << "GeneratePath unitXArg " << unitXArg << " unitYArg " << unitYArg << " destXArg " << destXArg << " destYArg " << destYArg << endl;
	if (GRAPH_TYPE != GRID && StraightLineIsOpen(unitXArg, unitYArg, destXArg, destYArg)) {
		stack<pair<double, double>> path;
		path.push(make_pair(destXArg, destYArg));
		cout << "Straight line path found" << endl;
		return path;
	}
	return AStar(unitXArg, unitYArg, destXArg, destYArg);
}

stack<pair<double, double>> Pathfinder::AStar(double unitXArg, double unitYArg, double destXArg, double destYArg) {
	cout << "AStar unitXArg " << unitXArg << " unitYArg " << unitYArg << " destXArg " << destXArg << " destYArg " << destYArg << endl;
	//cout << "Generating A* path from ";
	ResetExploration();
	priority_queue<ExploredNode, vector<ExploredNode>, CompareCost> exploredNodesQueue;
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

	//cout << startNode.x << ", " << startNode.y << " to " << destCellX << ", " << destCellY << ". Euclidean distance: " << startNode.totalCost << endl;

	// Destination variables for visibility graphs
	int destCellXSection;
	int destCellYSection;
	int destCellSectionXMin;
	int destCellSectionXMax;
	int destCellSectionYMin;
	int destCellSectionYMax;
	bool destIsInVisibilityGraph;
	if (GRAPH_TYPE != GRID) {
		destCellXSection = destCellX / visibilitySectionWidth;
		destCellYSection = destCellY / visibilitySectionHeight;
		destCellSectionXMin = destCellXSection * visibilitySectionWidth;
		destCellSectionXMax = destCellSectionXMin + visibilitySectionWidth;
		destCellSectionYMin = destCellYSection * visibilitySectionWidth;
		destCellSectionYMax = destCellSectionYMin + visibilitySectionHeight;
		destIsInVisibilityGraph = IsWallNode(destCellX, destCellY) || IsVisibilityNode(destCellX, destCellY, destCellXSection, destCellYSection);
	}

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
		
		// Check if goal node can reach visited node when using visibility graphs
		if (GRAPH_TYPE != GRID
			&& !destIsInVisibilityGraph
			//&& ((currentX / visibilitySectionWidth == destCellXSection
			//&& currentY / visibilitySectionHeight == destCellYSection)
			&& currentX >= destCellSectionXMin
			&& currentX <= destCellSectionXMax
			&& currentY >= destCellSectionYMin
			&& currentY <= destCellSectionYMax) {

			for (set<pair<pair<int, int>, double>>::iterator it = nodes[destCellX][destCellY].neighbors.begin(); it != nodes[destCellX][destCellY].neighbors.end(); ) {
				if (it->first.first == currentX && it->first.second == currentY) {
					visitedNodes[destCellX][destCellY].cost = visitedNodes[currentX][currentY].cost + math.CellDistance(currentX, currentY, destCellX, destCellY);
					visitedNodes[destCellX][destCellY].cameFromX = currentX;
					visitedNodes[destCellX][destCellY].cameFromY = currentY;
					currentX = destCellX;
					currentY = destCellY;
					break;
				}
				++it;
			}
		}

		// Explore nearby nodes
		if (currentX != destCellX || currentY != destCellY) {
			for (set<pair<pair<int, int>, double>>::iterator it = nodes[currentX][currentY].neighbors.begin(); it != nodes[currentX][currentY].neighbors.end(); ) {
			//for (int i = 0; i < nodes[currentX][currentY].neighbors.size(); i++) {

				int neighborX = it->first.first;
				int neighborY = it->first.second;

				if (visitedNodes[neighborX][neighborY].cost == -1.0
					&& exploredNodesMap[neighborX][neighborY] == false) {

					ExploredNode exploredNode;
					exploredNode.x = neighborX;
					exploredNode.y = neighborY;
					exploredNode.cameFromX = currentX;
					exploredNode.cameFromY = currentY;
					exploredNode.costFromStart = visitedNodes[currentX][currentY].cost + it->second;
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
				nodes[xArg + a][yArg + b].neighbors.erase(make_pair(make_pair(xArg, yArg), edgeWeight));

				// If cell and neighbor is open and neighbor is horizontal/vertical or diagonal with open space in between
				if (pMap->getCellStatus(xArg, yArg) == pMap->OPEN
					&& pMap->getCellStatus(xArg + a, yArg + b) == pMap->OPEN
					&& pMap->getCellStatus(xArg + a, yArg) == pMap->OPEN
					&& pMap->getCellStatus(xArg, yArg + b) == pMap->OPEN) {

					// Add neighbor to cell
					nodes[xArg][yArg].neighbors.insert(make_pair(make_pair(xArg + a, yArg + b), edgeWeight));

					// Add cell to neighbor's neighbor list
					nodes[xArg + a][yArg + b].neighbors.insert(make_pair(make_pair(xArg, yArg), edgeWeight));
				}
			}
		}
	}

	generation++;
}

void Pathfinder::CreateVisibilityGraph() {
	cout << "CreateVisibilityGraph" << endl;
	//cout << "Creating visibility graph for every section." << endl;

	nodes.resize(mapWidth, vector<Node>(mapHeight));
	visibilityNodes.resize(mapWidth, vector<bool>(mapHeight, false));
	/*visibilitySectionNodes.resize(visibilitySectionWidth, vector<std::set<std::pair<int, int>>>(visibilitySectionHeight));
	for (int i = 0; i < visibilitySectionWidth; i++) {
		for (int j = 0; j < visibilitySectionHeight; j++) {
			visibilitySectionNodes[i][j].clear();
		}
	}*/
	visibilitySectionNodes.resize(visibilityXSections, vector<std::set<std::pair<int, int>>>(visibilityYSections));
	for (int i = 0; i < visibilityXSections; i++) {
		for (int j = 0; j < visibilityYSections; j++) {
			visibilitySectionNodes[i][j].clear();
		}
	}

	for (int i = 0; i < visibilityXSections; i++) {
		for (int j = 0; j < visibilityYSections; j++) {
			UpdateGraph(i * visibilitySectionWidth, j * visibilitySectionHeight);
		}
	}
}

void Pathfinder::ClearVisibilitySectionNodes(int x0Arg, int y0Arg, int x1Arg, int y1Arg, int xSectionArg, int ySectionArg) {
	cout << "ClearVisibilitySectionNodes x0Arg " << x0Arg << " y0Arg " << y0Arg << " x1Arg " << x1Arg << " y1Arg " << y1Arg << endl;
	visibilitySectionNodes[xSectionArg][ySectionArg].clear();
	for (int x = x0Arg; x <= x1Arg; x++) {
		for (int y = y0Arg; y <= y1Arg; y++) {
			visibilityNodes[x][y] = false;
		}
	}
}

void Pathfinder::ClearSectionEdges(int neighborXMinArg, int neighborYMinArg, int neighborXMaxArg, int neighborYMaxArg, int xMinArg, int yMinArg, int xMaxArg, int yMaxArg) {
	
	// Clear all edges for cells in main section (including upper/left walls)
	for (int x = neighborXMinArg; x <= neighborXMaxArg; x++) {
		for (int y = neighborYMinArg; y <= neighborYMaxArg; y++) {
			if (x >= xMinArg
				&& y >= yMinArg
				&& x <= xMaxArg
				&& y <= yMaxArg) {

				nodes[x][y].neighbors.clear();
			}

			// Clear all edges going into main section (including upper/left walls)
			else {
				RemoveEdgesGoingToRect(x, y, xMinArg, yMinArg, xMaxArg, yMaxArg);
			}
		}
	}
}

void Pathfinder::ClearOutboundEdgesInRect(int xMinArg, int yMinArg, int xMaxArg, int yMaxArg) {
	cout << "ClearOutboundEdgesInRect xMinArg " << xMinArg << " yMinArg " << yMinArg << " xMaxArg " << xMaxArg << " yMaxArg " << yMaxArg << endl;
	for (int x = yMinArg; x <= xMaxArg; x++) {
		for (int y = yMinArg; y <= yMaxArg; y++) {
			if (pMap->IsLegalCell(x, y)) {
				nodes[x][y].neighbors.clear();
			}
		}
	}
}

void Pathfinder::ClearInnerWallEdges(int xWallMinArg, int yWallMinArg, int xWallMaxArg, int yWallMaxArg) {
	cout << "ClearInnerWallEdges xMinArg " << xWallMinArg << " yWallMin " << yWallMinArg << " xWallMax " << xWallMaxArg << " yWallMax " << yWallMaxArg << endl;
	for (int x = xWallMinArg; x <= xWallMaxArg; x++) {
		RemoveNeighborsOutsideRect(x, yWallMinArg, xWallMinArg, yWallMinArg, xWallMaxArg, yWallMaxArg);
		nodes[x][yWallMinArg].neighbors.clear();
	}
	for (int y = yWallMinArg + 1; y <= yWallMaxArg; y++) {
		RemoveNeighborsOutsideRect(xWallMinArg, y, xWallMinArg, yWallMinArg, xWallMaxArg, yWallMaxArg);
		nodes[xWallMinArg][y].neighbors.clear();
	}
}

void Pathfinder::ClearEdgesFromOuterWall(int xWallMinArg, int yWallMinArg, int xWallMaxArg, int yWallMaxArg) {
	cout << "ClearEdgesFromOuterWall xMinArg " << xWallMinArg << " yWallMin " << yWallMinArg << " xWallMax " << xWallMaxArg << " yWallMax " << yWallMaxArg << endl;
	for (int x = xWallMinArg; x <= xWallMaxArg; x++) {
		RemoveEdgesGoingToRect(x, yWallMinArg, xWallMinArg, yWallMinArg, xWallMaxArg - 1, yWallMaxArg - 1);
	}
	for (int y = yWallMinArg + 1; y <= yWallMaxArg; y++) {
		RemoveEdgesGoingToRect(xWallMinArg, y, xWallMinArg, yWallMinArg, xWallMaxArg, yWallMaxArg);
	}
}

void Pathfinder::RemoveNeighborsOutsideRect(int neighborXArg, int neighborYArg, int xMinArg, int yMinArg, int xMaxArg, int yMaxArg) {
	for (set<pair<pair<int, int>, double>>::iterator it = nodes[neighborXArg][neighborYArg].neighbors.begin(); it != nodes[neighborXArg][neighborYArg].neighbors.end(); ) {
		if (it->first.first < xMinArg
			|| it->first.second < yMinArg
			|| it->first.first > xMaxArg
			|| it->first.second > yMaxArg) {

			nodes[neighborXArg][neighborYArg].neighbors.erase(it);
		}
		++it;
	}
}

void Pathfinder::RemoveEdgesGoingToRect(int xArg, int yArg, int xMinArg, int yMinArg, int xMaxArg, int yMaxArg) {
	for (set<pair<pair<int, int>, double>>::iterator it = nodes[xArg][yArg].neighbors.begin(); it != nodes[xArg][yArg].neighbors.end(); ) {
		if (it->first.first >= xMinArg
			&& it->first.second >= yMinArg
			&& it->first.first <= xMaxArg
			&& it->first.second <= yMaxArg) {

			nodes[xArg][xArg].neighbors.erase(it);
		}
		++it;
	}
}

void Pathfinder::ClearVisibilitySectionEdges(int xSectionArg, int ySectionArg) {
	cout << "ClearVisibilitySectionEdges xSectionArg " << xSectionArg << " ySectionArg " << ySectionArg << endl;

	/*int destinationSectionXMin = destinationXSectionArg * visibilitySectionWidth;
	int destinationSectionXMax = destinationSectionXMin + visibilitySectionWidth - 1;
	int destinationSectionYMin = destinationYSectionArg * visibilitySectionHeight;
	int destinationSectionYMax = destinationSectionXMax + visibilitySectionHeight - 1;*/

	for (int i = xSectionArg * visibilitySectionWidth; i < (xSectionArg + 1) * visibilitySectionWidth; i++) {
		for (int j = ySectionArg * visibilitySectionHeight; j < (ySectionArg + 1) * visibilitySectionHeight; j++) {
			if (pMap->IsLegalCell(i, j)) {
				nodes[i][j].neighbors.clear();

				/*// Remove all edges in section going to nodes in destination section
				if (xSectionArg > destinationXSectionArg
					|| ySectionArg > destinationYSectionArg
					|| IsWallNode(i, j)) {

					// @TODO: Optimize: Remove elements from the old list rather than filling up a new list OR use vectors instead
					forward_list<pair<double, pair<int, int>>> edgesToOtherSections;
					for (forward_list<pair<double, pair<int, int>>>::iterator it = nodes[i][j].neighbors.begin(); it != nodes[i][j].neighbors.end(); ) {
						if (it->second.first < destinationSectionXMin
							|| it->second.first > destinationSectionXMax
							|| it->second.second < destinationSectionYMin
							|| it->second.second > destinationSectionYMax) {

							edgesToOtherSections.insert_after(edgesToOtherSections.before_begin(), *it);
						}
						++it;
					}
					nodes[i][j].neighbors = edgesToOtherSections;
				}*/
			}
		}
	}
}

void Pathfinder::ClearVisibilitySectionEdgesGoingTo(int xSection0Arg, int ySection0Arg, int xSection1Arg, int ySection1Arg) {
	cout << "ClearVisibilitySectionEdgesGoingTo xSection0Arg " << xSection0Arg << " ySection0Arg " << ySection0Arg << " xSection1Arg " << xSection1Arg << " ySection1Arg " << ySection1Arg << endl;

	int destinationSectionXMin = xSection1Arg * visibilitySectionWidth;
	int destinationSectionXMax = destinationSectionXMin + visibilitySectionWidth - 1;
	int destinationSectionYMin = ySection1Arg * visibilitySectionHeight;
	int destinationSectionYMax = destinationSectionXMax + visibilitySectionHeight - 1;

	for (int i = xSection0Arg * visibilitySectionWidth; i < (ySection0Arg + 1) * visibilitySectionWidth; i++) {
		for (int j = xSection1Arg * visibilitySectionHeight; j < (xSection1Arg + 1) * visibilitySectionHeight; j++) {
			if (pMap->IsLegalCell(i, j)) {
				//nodes[i][j].neighbors.clear();

				// Remove all edges in section going to nodes in destination section
				if (xSection0Arg > xSection1Arg
					|| ySection0Arg > ySection1Arg
					|| IsWallNode(i, j)) {

					// @TODO: Optimize: Remove elements from the old list rather than filling up a new list OR use vectors instead
					/*forward_list<pair<double, pair<int, int>>> edgesToOtherSections;
					for (forward_list<pair<double, pair<int, int>>>::iterator it = nodes[i][j].neighbors.begin(); it != nodes[i][j].neighbors.end(); ) {
						if (it->second.first < destinationSectionXMin
							|| it->second.first > destinationSectionXMax
							|| it->second.second < destinationSectionYMin
							|| it->second.second > destinationSectionYMax) {

							edgesToOtherSections.insert_after(edgesToOtherSections.before_begin(), *it);
						}
						++it;
					}
					nodes[i][j].neighbors = edgesToOtherSections;*/
				}
			}
		}
	}
}


void Pathfinder::CreateVisibilitySectionNodes(int x0Arg, int y0Arg, int x1Arg, int y1Arg, int xSectionArg, int ySectionArg) {
	cout << "CreateVisibilitySectionNodes xSectionArg " << xSectionArg << " ySectionArg " << ySectionArg << endl;
	for (int x = x0Arg; x <= x1Arg; x++) {
		for (int y = y0Arg; y <= y1Arg; y++) {
			CreateVisibilityNode(x, y, xSectionArg, ySectionArg);
		}
	}
}

void Pathfinder::CreateVisibilitySectionInternalEdges(int xMinArg, int yMinArg, int xMaxArg, int yMaxArg, int xSectionArg, int ySectionArg) {
	cout << "CreateVisibilitySectionInternalEdges" << endl;
	for (int x = xMinArg; x <= xMaxArg; x++) {
		for (int y = yMinArg; y <= yMaxArg; y++) {
			bool isVisibilityOrWallNode = IsVisibilityNode(x, y, xSectionArg, ySectionArg)
				|| x == xMinArg || y == yMinArg || x == xMaxArg || y == yMaxArg;
			CreateVisibilityEdges(x, y, xSectionArg, ySectionArg, isVisibilityOrWallNode);
			CreateVisibilityWallEdges(x, y, xMinArg, yMinArg, xMaxArg, yMaxArg, isVisibilityOrWallNode);
		}
	}
}

void Pathfinder::CreateVisibilitySectionExternalEdges(int neighborXMinArg, int neighborYMinArg, int xMinArg, int yMinArg, int xMaxArg, int yMaxArg, int xSectionArg, int ySectionArg) {
	cout << "CreateVisibilitySectionExternalEdges" << endl;

	for (int x = neighborXMinArg; x <= xMaxArg; x++) {
		for (int y = neighborYMinArg; y <= yMaxArg; y++) {

			// Limit checked range when reaching main section
			if (x == xMinArg) {
				yMaxArg = yMinArg - 1;
			}

			bool isVisibilityOrWallNode = x == neighborXMinArg
				|| x == xMinArg
				|| x == xMaxArg
				|| y == neighborYMinArg
				|| y == yMinArg
				|| y == yMaxArg;

			// Upper neighbor section
			if (x >= xMinArg) {
				if (!isVisibilityOrWallNode && IsVisibilityNode(x, y, xSectionArg, ySectionArg)) {
					isVisibilityOrWallNode = true;
				}
				for (int xWall = xMinArg; xWall <= xMaxArg; xWall++) {
					CreateEdge(x, y, xWall, yMinArg, isVisibilityOrWallNode);
				}
			}

			// Left neighbor section
			else if (y >= yMinArg) {
				if (!isVisibilityOrWallNode && IsVisibilityNode(x, y, xSectionArg, ySectionArg)) {
					isVisibilityOrWallNode = true;
				}
				for (int yWall = yMinArg; yWall <= yMaxArg; yWall++) {
					CreateEdge(x, y, yMinArg, yWall, isVisibilityOrWallNode);
				}
			}

			// Upper left neighbor section
			else {
				if (!isVisibilityOrWallNode && IsVisibilityNode(x, y, xSectionArg - 1, ySectionArg - 1)) {
					isVisibilityOrWallNode = true;
				}
				CreateEdge(x, y, xMinArg, yMinArg, isVisibilityOrWallNode);
			}
		}
	}
}

/*void Pathfinder::CreateVisibilitySectionEdges(int x0Arg, int y0Arg, int x1Arg, int y1Arg, int xSectionArg, int ySectionArg) {
	cout << "CreateVisibilitySectionEdges xSectionArg " << xSectionArg << " ySectionArg " << ySectionArg << endl;
	for (int x = x0Arg; x <= x1Arg; x++) {
		for (int y = y0Arg; y <= y1Arg; y++) {
			CreateVisibilityEdges(x, y, xSectionArg, ySectionArg);
		}
	}
}*/

/*void Pathfinder::CreateVisibilitySectionWallEdges(int xMinArg, int yMinArg, int xMaxArg, int yMaxArg, int xWallMinArg, int yWallMinArg, int xWallMaxArg, int yWallMaxArg) {
	cout << "CreateVisibilitySectionWallEdges" << endl;
	for (int x = xMinArg; x <= xMaxArg; x++) {
		for (int y = yMinArg; y <= yMaxArg; y++) {
			CreateVisibilityWallEdges(x, y, xWallMinArg, yWallMinArg, xWallMaxArg, yWallMaxArg);
		}
	}
}*/

/*void Pathfinder::CreateEdgesFromOuterSectionsToInnerWall(int xWallMinArg, int yWallMinArg, int xWallMaxArg, int yWallMaxArg, int neighborXMinArg, int neighborYMinArg, int neighborXMaxArg, int neighborYMaxArg) {
	
	// From upper wall in main section
	for (int x0 = xWallMinArg; x0 <= xWallMaxArg; x0++) {

		// - To upper wall in upper neighbor section
		for (int x1 = xWallMinArg; x1 <= xWallMaxArg; x1++) {
			CreateWallEdge(x0, yWallMinArg, x1, neighborYMinArg, xWallMinArg, neighborYMinArg, xWallMaxArg, yWallMinArg);
		}

		// - To left and right wall in upper neighbor section
		for (int y1 = neighborYMinArg + 1; y1 < yWallMinArg; y1++) {
			CreateWallEdge(x0, yWallMinArg, xWallMinArg, y1, xWallMinArg, neighborYMinArg, xWallMaxArg, yWallMinArg);
			CreateWallEdge(x0, yWallMinArg, neighborXMinArg, y1, xWallMinArg, neighborYMinArg, xWallMaxArg, yWallMinArg);
		}
	}

	// From left wall in main section
	for (int y0 = yWallMinArg; y0 <= yWallMaxArg; y0++) {

		// - To left wall in left neighbor section
		for (int y1 = yWallMinArg; y1 <= yWallMaxArg; y1++) {
			CreateWallEdge(xWallMinArg, y0, neighborXMinArg, y1, neighborXMinArg, yWallMinArg, xWallMinArg, yWallMaxArg);
		}

		// - To upper and lower wall in left neighbor section
		for (int x1 = neighborXMinArg + 1; x1 < xWallMinArg; x1++) {
			CreateWallEdge(xWallMinArg, y0, x1, yWallMinArg, neighborXMinArg, yWallMinArg, xWallMinArg, yWallMaxArg);
			CreateWallEdge(xWallMinArg, y0, x1, neighborYMinArg, neighborXMinArg, yWallMinArg, xWallMinArg, yWallMaxArg);
		}
	}

	// From upper right wall cell in main section
	CreateVisibilityWallEdges(xWallMaxArg, yWallMinArg, xWallMaxArg, neighborYMinArg, xWallMaxArg + visibilitySectionWidth, yWallMinArg);

	// From lower left wall cell in main section
	CreateVisibilityWallEdges(xWallMinArg, yWallMaxArg, neighborXMinArg, yWallMaxArg, xWallMinArg, yWallMaxArg + visibilitySectionHeight);

	// From upper left wall cell in main section
	CreateVisibilityWallEdges(xWallMinArg, yWallMinArg, xWallMinArg, yWallMinArg, xWallMinArg, yWallMinArg);
}*/

/*void Pathfinder::CreateVisibilitySectionWallEdgesGoingTo(int xSection0Arg, int ySection0Arg, int xSection1Arg, int ySection1Arg) {
	cout << "CreateVisibilitySectionWallEdgesGoingTo xSection0Arg " << xSection0Arg << " ySection0Arg " << ySection0Arg << " xSection1Arg " << xSection1Arg << " ySection1Arg " << ySection1Arg << endl;
	for (int i = xSection0Arg * (visibilitySectionWidth); i <= (xSection0Arg + 1) * visibilitySectionWidth; i++) {
		for (int j = ySection0Arg * (visibilitySectionHeight); j <= (ySection0Arg + 1) * visibilitySectionHeight; j++) {
			if (pMap->IsLegalCell(i, j)) {
				CreateVisibilityWallEdgesGoingTo(i, j, xSection1Arg, ySection1Arg);
			}
		}
	}
}*/

void Pathfinder::CreateVisibilityNode(int xArg, int yArg, int xSectionArg, int ySectionArg) {
	//cout << "CreateVisibilityNode xSectionArg " << xSectionArg << " ySectionArg " << ySectionArg << " xArg " << xArg << " yArg " << yArg << endl;
	if (IsAtCorner(xArg, yArg)) {
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

void Pathfinder::CreateVisibilityEdges(int xArg, int yArg, int xSectionArg, int ySectionArg, bool bidirectedEdgeArg) {
	//cout << "CreateVisibilityEdges xSectionArg " << xSectionArg << " ySectionArg " << ySectionArg << " xArg " << xArg << " yArg " << yArg << endl;
	double edgeWeight;

	// Find reachable visibility nodes in section
	set<pair<int, int>>::iterator it = visibilitySectionNodes.at(xSectionArg).at(ySectionArg).begin();
	while (it != visibilitySectionNodes.at(xSectionArg).at(ySectionArg).end()) {
		edgeWeight = math.CellDistance(xArg, yArg, it->first, it->second);

		if (it->first != xArg || it->second != yArg
			&& nodes[xArg][yArg].neighbors.find(make_pair(make_pair(it->first, it->second), edgeWeight)) == nodes[xArg][yArg].neighbors.end()
			&& StraightLineIsOpen(xArg, yArg, it->first, it->second)) {
			
			// Create (bidirected) edge
			nodes[xArg][yArg].neighbors.insert(make_pair(make_pair(it->first, it->second), edgeWeight));
			if (bidirectedEdgeArg) {
				nodes[it->first][it->second].neighbors.insert(make_pair(make_pair(xArg, yArg), edgeWeight));
			}
		}
		++it;
	}

	generation++;
}

void Pathfinder::CreateVisibilityWallEdges(int xArg, int yArg, int xWallMinArg, int yWallMinArg, int xWallMaxArg, int yWallMaxArg, bool bidirectedEdgeArg) {
	//cout << "CreateVisibilityWallEdges xSectionArg " << xSectionArg << " ySectionArg " << ySectionArg << " xArg " << xArg << " yArg " << yArg << endl;

	for (int x = xWallMinArg; x <= xWallMaxArg; x++) {
		CreateEdge(xArg, yArg, x, yWallMinArg, bidirectedEdgeArg);
		CreateEdge(xArg, yArg, x, yWallMaxArg, bidirectedEdgeArg);
	}
	for (int y = xWallMinArg + 1; y < xWallMaxArg; y++) {
		CreateEdge(xArg, yArg, xWallMinArg, y, bidirectedEdgeArg);
		CreateEdge(xArg, yArg, xWallMaxArg, y, bidirectedEdgeArg);
	}

	generation++;

	// Find reachable visibility graph section wall nodes
	/*int xSection = xArg / visibilitySectionWidth;
	int ySection = yArg / visibilitySectionHeight;
	int xMin = xSection * visibilitySectionWidth;
	int yMin = ySection * visibilitySectionHeight;
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

	// Find reachable visibility graph section visibility nodes
	if (GRAPH_TYPE == VISIBILITY_DECOMPOSED) {
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
	}

	generation++;*/
}

/*void Pathfinder::CreateVisibilityWallEdgesGoingTo(int xArg, int yArg, int xSectionDestinationArg, int ySectionDestinationArg) {
	
	// Connect cells in neighbor section to one of the walls of the origin/destination section
	int xSection = xArg / visibilitySectionWidth;
	int ySection = yArg / visibilitySectionHeight;
	if (xSection == xSectionDestinationArg || ySection == ySectionDestinationArg) {
		// @TODO: Get coordinates as arguments
		int xMin = xSectionDestinationArg * visibilitySectionWidth;
		int yMin = ySectionDestinationArg * visibilitySectionHeight;
		int xMax = xMin + visibilitySectionWidth;
		int yMax = yMin + visibilitySectionHeight;
		if (xSection < xSectionDestinationArg) {
			yMax = yMin;
		}
		else if (xSection > xSectionDestinationArg) {
			yMin = yMax;
		}
		else if (ySection < ySectionDestinationArg) {
			xMax = xMin;
		}
		else {
			xMin = xMax;
		}
		for (int x = xMin; x <= xMax; x++) {
			for (int y = yMin; y <= yMax; y++) {
				CreateWallEdge(xArg, yArg, x, y);
			}
		}
	}

	// Connect cells in neighbor section to corner of origin/destination section
	else {
		if (xSection < xSectionDestinationArg) {
			if (ySection < ySectionDestinationArg) {
				// @TODO: Get coordinates as arguments
				CreateWallEdge(xArg, yArg, xSectionDestinationArg * visibilitySectionWidth, ySectionDestinationArg * visibilitySectionHeight);
			}
			else {
				CreateWallEdge(xArg, yArg, xSectionDestinationArg * visibilitySectionWidth, (ySectionDestinationArg + 1) * visibilitySectionHeight);
			}
		}
		else if (ySection < ySectionDestinationArg) {
			CreateWallEdge(xArg, yArg, (xSectionDestinationArg + 1) * visibilitySectionWidth, ySectionDestinationArg * visibilitySectionHeight);
		}
		else {
			CreateWallEdge(xArg, yArg, (xSectionDestinationArg + 1) * visibilitySectionWidth, (ySectionDestinationArg + 1) * visibilitySectionHeight);
		}
	}

	generation++;
}*/

void Pathfinder::CreateEdge(int x0Arg, int y0Arg, int x1Arg, int y1Arg, bool bidirectedEdgeArg) {

	if (pMap->getCellStatus(x0Arg, y0Arg) == pMap->OPEN
		&& pMap->getCellStatus(x1Arg, y1Arg) == pMap->OPEN
		&& (x0Arg != x1Arg || y0Arg != y1Arg)) {
		
		int edgeWeight = math.CellDistance(x0Arg, y0Arg, x1Arg, y1Arg);

		if (nodes[x0Arg][y0Arg].neighbors.find(make_pair(make_pair(x1Arg, y0Arg), edgeWeight)) == nodes[x0Arg][y0Arg].neighbors.end()
			&& StraightLineIsOpen(x0Arg, y0Arg, x1Arg, y1Arg)) {

			nodes[x0Arg][y0Arg].neighbors.insert(make_pair(make_pair(x1Arg, y1Arg), edgeWeight));
			if (bidirectedEdgeArg) {
				nodes[x1Arg][y1Arg].neighbors.insert(make_pair(make_pair(x0Arg, y0Arg), edgeWeight));
			}
		}
	}
}

bool Pathfinder::IsVisibilityNode(int xArg, int yArg, int xSectionArg, int ySectionArg) {
	set<pair<int, int>>::iterator it = visibilitySectionNodes.at(xSectionArg).at(ySectionArg).find(std::make_pair(xArg, yArg));
	return it != visibilitySectionNodes.at(xSectionArg).at(ySectionArg).end();
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
