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
	bool CellTouchingCellOfType(CellStatus typeArg, int xArg, int yArg);
	bool getInitialized();
	int getWidth();
	int getHeight();
	CellStatus getCellStatus(int xArg, int yArg);
	void setCellStatus(CellStatus cellStatusArg, int xArg, int yArg);
	bool getMapUpdated();
	void setMapUpdated(bool mapUpdatedArg);
	std::vector<std::vector<CellStatus>>* getCellStatusArrayPtr();

private:
	bool loaded;
	const char* name;
	unsigned int width;
	unsigned int height;
	std::vector<std::vector<CellStatus>> cellStatusArray;
	bool initialized;
	bool mapUpdated;
};

