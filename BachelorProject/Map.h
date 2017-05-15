/*
Map.h
Created by Martin Dariush Hansen, 2017-03-14
*/

#pragma once
#include <vector>

class Map
{
public:
	enum CellStatus { OPEN, CLOSED };

	Map();
	~Map();
	void Init(const char* mapName);
	void LoadMapImage();
	bool IsLegalCell(int x, int y);
	bool CellBlocking(int xArg, int yArg);
	bool CellTouchingCellOfType(CellStatus typeArg, int xArg, int yArg);
	void Clear(CellStatus cellStatusArg);
	void Reset(int widthArg, int heightArg, CellStatus statusArg);
	bool getInitialized();
	int getWidth();
	int getHeight();
	CellStatus getCellStatus(int xArg, int yArg);
	void setCellStatus(CellStatus cellStatusArg, int xArg, int yArg);
	int getGeneration();
	void IncrementGeneration();
	std::vector<std::vector<CellStatus>>* getCellStatusArrayPtr();

private:
	bool loaded;
	const char* name;
	unsigned int width;
	unsigned int height;
	std::vector<std::vector<CellStatus>> cellStatusArray;
	bool initialized;
	int generation;
	const static std::vector<std::pair <int, int>> cellSurroundChecking;
};

