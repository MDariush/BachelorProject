/*
Pathfinder.h
Created by Martin Dariush Hansen, 2017-04-13
*/

#pragma once
#include "Mathematics.h"
#include <forward_list>
#include <stack>
#include <set>

class Pathfinder
{
public:
	struct CompareCoordinates {
		constexpr bool operator()(std::pair<std::pair<int, int>, double> const & a, std::pair<std::pair<int, int>, double> const & b) const noexcept {
			return a.first.first < b.first.first || (!(b.first.first < a.first.first) && a.first.second < b.first.second);
		}
	};
	struct Node {
		std::set<std::pair<std::pair<int, int>, double>, CompareCoordinates> neighbors;
		//std::forward_list<std::pair<double, std::pair<int, int>>> neighbors;
	};
	void Init(class Vision* pVisionArg);
	void UpdateGraph(int xArg, int yArg);
	std::stack<std::pair<double, double>> GeneratePath(double unitXArg, double unitYArg, double destXArg, double destYArg);
	std::vector<std::vector<Node>>* getNodesPtr();
	int getGeneration();
	void setGeneration(int generationArg);

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

	std::stack<std::pair<double, double>> Pathfinder::AStar(double unitXArg, double unitYArg, double destXArg, double destYArg);
	void ResetExploration();
	void CreateGridGraph();
	void UpdateGridGraphNode(int xArg, int yArg);
	void CreateVisibilityGraph();
	void ClearVisibilitySectionNodes(int x0Arg, int y0Arg, int x1Arg, int y1Arg, int xSectionArg, int ySectionArg);
	void ClearSectionEdges(int neighborXMinArg, int neighborYMinArg, int neighborXMaxArg, int neighborYMaxArg, int xMinArg, int yMinArg, int xMaxArg, int yMaxArg);
	void ClearEdgesIntersectingSection(int sectionX0Arg, int sectionY0Arg, int sectionX1Arg, int sectionY1Arg, int boundaryX0Arg, int boundaryY0Arg, int boundaryX1Arg, int boundaryY1Arg, int neighborXMinArg, int neighborYMinArg);
	void ClearEdgesInRect(int x0Arg, int y0Arg, int x1Arg, int y1Arg);
	void ClearInnerWallEdges(int xMinArg, int yMinArg, int xMaxArg, int yMaxArg);
	void ClearEdgesFromOuterWall(int xWallMinArg, int yWallMinArg, int xWallMaxArg, int yWallMaxArg);
	void RemoveNeighborsOutsideRect(int neighborXArg, int neighborYArg, int xMinArg, int yMinArg, int xMaxArg, int yMaxArg);
	void RemoveEdgesGoingToRect(int xArg, int yArg, int xMinArg, int yMinArg, int xMaxArg, int yMaxArg);
	void CreateEdgesFromRectToPoint(int rectX0Arg, int rectY0Arg, int rectX1Arg, int rectY1Arg, int xPointArg, int yPointArg, int xSectionArg, int ySectionArg);
	void CreateEdgesFromWallToWall(int rect0X0Arg, int rect0Y0Arg, int rect0X1Arg, int rect0Y1Arg, int rect1X0Arg, int rect1Y0Arg, int rect1X1Arg, int rect1Y1Arg);
	void RemoveEdgesFromRectToPoint(int rectX0Arg, int rectY0Arg, int rectX1Arg, int rectY1Arg, int xPointArg, int yPointArg);
	void RemoveEdgesFromRectToRect(int rect0X0Arg, int rect0Y0Arg, int rect0X1Arg, int rect0Y1Arg, int rect1X0Arg, int rect1Y0Arg, int rect1X1Arg, int rect1Y1Arg);
	void ClearVisibilitySectionEdges(int xSectionArg, int ySectionArg);
	//void ClearVisibilitySection(int xArg, int yArg, int xSectionArg, int ySectionArg);
	//void ClearVisibilitySectionGoingTo(int xArg, int yArg, int xSectionArg, int ySectionArg, int destinationXSectionArg, int destinationYSectionArg);
	void CreateVisibilitySectionNodes(int x0Arg, int y0Arg, int x1Arg, int y1Arg, int xSectionArg, int ySectionArg);
	void CreateVisibilityEdgesInRect(int xMinArg, int yMinArg, int xMaxArg, int yMaxArg, int xSectionArg, int ySectionArg);
	void CreateVisibilitySectionInternalEdges(int xMinArg, int yMinArg, int xMaxArg, int yMaxArg, int xSectionArg, int ySectionArg);
	void CreateVisibilitySectionExternalEdges(int neighborXMinArg, int neighborYMinArg, int xMinArg, int yMinArg, int xMaxArg, int yMaxArg, int xSectionArg, int ySectionArg);
	void CreateEdgesIntersectingSection(int sectionX0Arg, int sectionY0Arg, int sectionX1Arg, int sectionY1Arg, int boundaryX0Arg, int boundaryY0Arg, int boundaryX1Arg, int boundaryY1Arg, int neighborXMinArg, int neighborYMinArg, int xSectionArg, int ySectionArg);
	//void CreateVisibilitySectionEdges(int x0Arg, int y0Arg, int x1Arg, int y1Arg, int xSectionArg, int ySectionArg);
	//void CreateVisibilitySectionEdges(int xSectionArg, int ySectionArg);
	//void CreateVisibilitySectionWallEdges(int xMinArg, int yMinArg, int xMaxArg, int yMaxArg, int xWallMinArg, int yWallMinArg, int xWallMaxArg, int yWallMaxArg);
	//void CreateEdgesFromOuterSectionsToInnerWall(int xWallMinArg, int yWallMinArg, int xWallMaxArg, int yWallMaxArg, int neighborXMinArg, int neighborYMinArg, int neighborXMaxArg, int neighborYMaxArg);
	//void CreateVisibilitySectionWallEdgesGoingTo(int xSection0Arg, int ySection0Arg, int xSection1Arg, int ySection1Arg);
	//void UpdateVisibilitySection(int xArg, int yArg);
	void CreateVisibilityNode(int xArg, int yArg, int xSectionArg, int ySectionArg);
	//void CreateVisibilityEdges(int xSectionArg, int ySectionArg, int xArg, int yArg);
	void CreateVisibilityEdges(int xArg, int yArg, int xSectionArg, int ySectionArg, bool bidirectedEdgeArg);
	void CreateVisibilityWallEdges(int xArg, int yArg, int xWallMinArg, int yWallMinArg, int xWallMaxArg, int yWallMaxArg, bool bidirectedEdgeArg);
	//void CreateVisibilityWallEdgesGoingTo(int xArg, int yArg, int xSectionDestinationArg, int ySectionDestinationArg);
	bool IsAtCorner(int xArg, int yArg);
	bool StraightLineIsOpen(int x0Arg, int y0Arg, int x1Arg, int y1Arg);
	void CreateEdge(int x0Arg, int y0Arg, int x1Arg, int y1Arg, bool bidirectedEdgeArg);
	bool IsVisibilityNode(int xArg, int yArg, int xSectionArg, int ySectionArg);
	bool IsWallNode(int xArg, int yArg);

	std::vector<std::vector<Node>> nodes;
	std::vector<std::vector<bool>> visibilityNodes;
	std::vector<std::vector<std::set<std::pair<int, int>>>> visibilitySectionNodes; // List of visibility nodes in each visibility section
	std::vector<std::vector<VisitedNode>> visitedNodes;
	std::vector<std::vector<bool>> exploredNodesMap;
	Mathematics math;
	class Map* pMap;
	class Vision* pVision;
	int mapWidth;
	int mapHeight;
	int generation;
	int visibilityXSections;
	int visibilityYSections;
	int visibilitySectionWidth;
	int visibilitySectionHeight;
};
