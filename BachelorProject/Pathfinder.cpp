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
	generation = 0;
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
	generation = 1;
	ResetExploration();
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
		if (generation == 0) {
			ClearVisibilitySectionNodes(xMin, yMin, xMax, yMax, xSection, ySection);
			ClearSectionEdges(neighborXMin, neighborYMin, xWallMax, yWallMax, xWallMin, yWallMin, xMax, yMax);
			ClearEdgesIntersectingSection(xWallMin, yWallMin, xMax, yMax, xWallMin - 1, yWallMin - 1, xWallMax, yWallMax, neighborXMin, neighborYMin);
			CreateVisibilitySectionNodes(xMin, yMin, xMax, yMax, xSection, ySection);
			CreateVisibilitySectionInternalEdges(xWallMin, yWallMin, xWallMax, yWallMax, xSection, ySection);
			CreateVisibilitySectionExternalEdges(neighborXMin, neighborYMin, xWallMin, yWallMin, xWallMax, yWallMax, xSection, ySection);
			generation = 0;
		}
		else {
			ClearVisibilitySectionNodes(xMin, yMin, xMax, yMax, xSection, ySection);
			ClearSectionEdges(neighborXMin, neighborYMin, xWallMax, yWallMax, xWallMin, yWallMin, xMax, yMax);
			ClearEdgesIntersectingSection(xWallMin, yWallMin, xMax, yMax, xWallMin - 1, yWallMin - 1, xWallMax, yWallMax, neighborXMin, neighborYMin);
		}
		break;
	case VISIBILITY_FULL:
		ClearVisibilitySectionNodes(0, 0, mapWidth - 1, mapHeight - 1, 0, 0);
		ClearOutboundEdgesInRect(xMin, yMin, xMax, yMax);

		CreateVisibilitySectionNodes(0, 0, mapWidth - 1, mapHeight - 1, 0, 0);
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
	cout << "ClearSectionEdges neighborXMinArg " << neighborXMinArg << " neighborYMinArg " << neighborYMinArg << " neighborXMaxArg " << neighborXMaxArg << " neighborYMaxArg " << neighborYMaxArg << " xMinArg " << xMinArg << " yMinArg " << yMinArg << " xMaxArg " << xMaxArg << " yMaxArg " << yMaxArg << endl;

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

void Pathfinder::ClearEdgesIntersectingSection(int sectionX0Arg, int sectionY0Arg, int sectionX1Arg, int sectionY1Arg, int boundaryX0Arg, int boundaryY0Arg, int boundaryX1Arg, int boundaryY1Arg, int neighborXMinArg, int neighborYMinArg) {
	cout << "ClearEdgesIntersectingSection sectionX0Arg " << sectionX0Arg << " sectionY0Arg " << sectionY0Arg << " sectionX1Arg " << sectionX1Arg << " sectionY1Arg " << sectionY1Arg << " boundaryX0Arg " << boundaryX0Arg << " boundaryY0Arg " << boundaryY0Arg << " boundaryX1Arg " << boundaryX1Arg << " boundaryY1Arg " << boundaryY1Arg << " neighborXMinArg " << neighborXMinArg << " neighborYMinArg " << neighborYMinArg << endl;

	// Remove edges from left section to left upper corner of lower section and vice versa
	RemoveEdgesFromRectToPoint(neighborXMinArg, sectionY0Arg, boundaryX0Arg, sectionY1Arg, sectionX0Arg, boundaryY1Arg);
	//RemoveEdgesGoingToRect(sectionX0Arg, boundaryY1Arg, neighborXMinArg, sectionY0Arg, boundaryX0Arg, sectionY1Arg);

	// Remove edges from upper section to left upper corner of right section and vice versa
	RemoveEdgesFromRectToPoint(sectionX0Arg, neighborYMinArg, sectionX1Arg, boundaryY0Arg, boundaryX1Arg, sectionY0Arg);
	//RemoveEdgesGoingToRect(boundaryX1Arg, sectionY0Arg, sectionX0Arg, neighborYMinArg, sectionX1Arg, boundaryY0Arg);

	// Remove edges from upper wall of lower section to left wall of right section and vice versa
	RemoveEdgesFromRectToRect(sectionX0Arg, boundaryY1Arg, boundaryX1Arg, boundaryY1Arg, neighborXMinArg, sectionY0Arg, boundaryX1Arg, sectionY1Arg);
	RemoveEdgesFromRectToRect(boundaryX1Arg, sectionY0Arg, boundaryX1Arg, boundaryY1Arg, sectionX0Arg, neighborYMinArg, sectionX1Arg, boundaryY1Arg);

	// Remove edges from lower to right wall of upper/left section and vice versa
	RemoveEdgesFromRectToRect(neighborXMinArg, sectionY0Arg, boundaryX0Arg, sectionY0Arg, sectionX0Arg, neighborYMinArg, sectionX0Arg, boundaryY0Arg);
	RemoveEdgesFromRectToRect(sectionX0Arg, neighborYMinArg, sectionX0Arg, boundaryY0Arg, neighborXMinArg, sectionY0Arg, boundaryX0Arg, sectionY0Arg);
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
	if (pMap->IsLegalCell(xArg, yArg)) {
		//cout << "RemoveEdgesGoingToRect xArg " << xArg << " yArg " << yArg << " xMinArg " << xMinArg << " yMinArg " << yMinArg << " xMaxArg " << xMaxArg << " yMaxArg " << yMaxArg << endl;

		for (set<pair<pair<int, int>, double>>::iterator it = nodes[xArg][yArg].neighbors.begin(); it != nodes[xArg][yArg].neighbors.end(); ) {
			if (it->first.first >= xMinArg
				&& it->first.second >= yMinArg
				&& it->first.first <= xMaxArg
				&& it->first.second <= yMaxArg) {

				it = nodes[xArg][yArg].neighbors.erase(it);
			}
			else {
				++it;
			}
		}
	}
}

void Pathfinder::RemoveEdgesFromRectToPoint(int rectX0Arg, int rectY0Arg, int rectX1Arg, int rectY1Arg, int xPointArg, int yPointArg) {
	cout << "RemoveEdgesFromRectToPoint rectX0Arg " << rectX0Arg << " rectY0Arg " << rectY0Arg << " rectX1Arg " << rectX1Arg << " rectY1Arg " << rectY1Arg << " xPointArg " << xPointArg << " yPointArg " << yPointArg << endl;
	for (int x = rectX0Arg; x <= rectX1Arg; x++) {
		for (int y = rectY0Arg; y <= rectY1Arg; y++) {
			cout << "Erase edge from " << x << ", " << y << " to " << xPointArg << ", " << yPointArg << " with distance " << math.CellDistance(x, y, xPointArg, yPointArg) << endl;
			//nodes[x][y].neighbors.erase(make_pair(make_pair(x, y), math.CellDistance(x, y, xPointArg, yPointArg)));
			nodes[x][y].neighbors.erase(make_pair(make_pair(xPointArg, yPointArg), 0));
		}
	}
}

void Pathfinder::RemoveEdgesFromRectToRect(int rect0X0Arg, int rect0Y0Arg, int rect0X1Arg, int rect0Y1Arg, int rect1X0Arg, int rect1Y0Arg, int rect1X1Arg, int rect1Y1Arg) {
	for (int x = rect0X0Arg; x <= rect0X1Arg; x++) {
		for (int y = rect0Y0Arg; y <= rect0Y1Arg; y++) {
			RemoveEdgesGoingToRect(x, y, rect1X0Arg, rect1Y0Arg, rect1X1Arg, rect1Y1Arg);
		}
	}
}

void Pathfinder::ClearVisibilitySectionEdges(int xSectionArg, int ySectionArg) {
	cout << "ClearVisibilitySectionEdges xSectionArg " << xSectionArg << " ySectionArg " << ySectionArg << endl;

	for (int i = xSectionArg * visibilitySectionWidth; i < (xSectionArg + 1) * visibilitySectionWidth; i++) {
		for (int j = ySectionArg * visibilitySectionHeight; j < (ySectionArg + 1) * visibilitySectionHeight; j++) {
			if (pMap->IsLegalCell(i, j)) {
				nodes[i][j].neighbors.clear();
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
	cout << "CreateVisibilitySectionInternalEdges xMinArg " << xMinArg << " yMinArg " << yMinArg << " xMaxArg " << xMaxArg << " yMaxArg " << yMaxArg << " xSectionArg " << xSectionArg << " ySectionArg " << ySectionArg << endl;
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
	cout << "CreateVisibilitySectionExternalEdges neighborXMinArg " << neighborXMinArg << " neighborYMinArg " << neighborYMinArg << " xMinArg " << xMinArg << " yMinArg " << yMinArg << " xMaxArg " << xMaxArg << " yMaxArg " << yMaxArg << " xSectionArg " << xSectionArg << " ySectionArg " << ySectionArg << endl;

	int yMax = yMaxArg;
	for (int x = neighborXMinArg; x <= xMaxArg; x++) {
		if (x == xMinArg) {
			yMax = yMinArg - 1;
		}
		for (int y = neighborYMinArg; y <= yMax; y++) {

			bool isVisibilityOrWallNode = x == neighborXMinArg
				|| x == xMinArg
				|| x == xMaxArg
				|| y == neighborYMinArg
				|| y == yMinArg
				|| y == yMaxArg;
			if (!isVisibilityOrWallNode && IsVisibilityNode(x, y, xSectionArg, ySectionArg)) {
				isVisibilityOrWallNode = true;
			}

			// Upper neighbor section
			if (x >= xMinArg) {
				for (int xWall = xMinArg; xWall <= xMaxArg; xWall++) {
					CreateEdge(x, y, xWall, yMinArg, isVisibilityOrWallNode);
				}
			}

			// Left neighbor section
			else if (y >= yMinArg) {
				for (int yWall = yMinArg; yWall <= yMaxArg; yWall++) {
					CreateEdge(x, y, xMinArg, yWall, isVisibilityOrWallNode);
				}
			}

			// Upper left neighbor section
			else {
				CreateEdge(x, y, xMinArg, yMinArg, isVisibilityOrWallNode);
			}
		}
	}
}

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
	//cout << "CreateVisibilityEdges xArg " << xArg << " yArg " << yArg << " xSectionArg " << xSectionArg << " ySectionArg " << ySectionArg << " bidirectedEdgeArg " << bidirectedEdgeArg << endl;
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
	//cout << "CreateVisibilityWallEdges xArg " << xArg << " yArg " << yArg << " xWallMinArg " << xWallMinArg << " yWallMinArg " << yWallMinArg << " xWallMaxArg " << xWallMaxArg << " yWallMaxArg " << yWallMaxArg << " bidirectedEdgeArg " << bidirectedEdgeArg << endl;

	for (int x = xWallMinArg; x <= xWallMaxArg; x++) {
		CreateEdge(xArg, yArg, x, yWallMinArg, bidirectedEdgeArg);
		CreateEdge(xArg, yArg, x, yWallMaxArg, bidirectedEdgeArg);
	}
	for (int y = yWallMinArg + 1; y < yWallMaxArg; y++) {
		CreateEdge(xArg, yArg, xWallMinArg, y, bidirectedEdgeArg);
		CreateEdge(xArg, yArg, xWallMaxArg, y, bidirectedEdgeArg);
	}

	generation++;
}

void Pathfinder::CreateEdge(int x0Arg, int y0Arg, int x1Arg, int y1Arg, bool bidirectedEdgeArg) {
	if (pMap->IsLegalCell(x0Arg, y0Arg) && pMap->IsLegalCell(x1Arg, y1Arg)
		&& (x0Arg != x1Arg || y0Arg != y1Arg)
		&& pMap->getCellStatus(x0Arg, y0Arg) == pMap->OPEN
		&& pMap->getCellStatus(x1Arg, y1Arg) == pMap->OPEN) {

		//cout << "CreateEdge x0Arg " << x0Arg << " y0Arg " << y0Arg << " x1Arg " << x1Arg << " y1Arg " << y1Arg << " bidirectedEdgeArg " << bidirectedEdgeArg << endl;

		int edgeWeight = math.CellDistance(x0Arg, y0Arg, x1Arg, y1Arg);

		if (nodes[x0Arg][y0Arg].neighbors.find(make_pair(make_pair(x1Arg, y1Arg), edgeWeight)) == nodes[x0Arg][y0Arg].neighbors.end()
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
				return false;
			}
			x += xLoopSpd;
		}
	}

	// @TODO: Merge the collision checking into one method
	// Check collision - Vertically
	else if (xSpd == 0.0) {
		while ((int)y != y1Arg) {
			if (pMap->getCellStatus(x, y) == Map::CLOSED) {
				return false;
			}
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
					return false;
				}
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
					return false;
				}
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
