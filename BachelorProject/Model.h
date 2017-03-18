/*
Model.h
Created by Martin Dariush Hansen, 2017-03-14
*/

#pragma once
#include "Game.h"
#include "Map.h"

class Model
{
public:
	enum Status { LOADING_MAP, IN_MAP };

	Model();
	~Model();

	void Init(class Graphics* graphics);
	void Step();
	Status getStatus();
	Map* getMapPtr();

private:
	void LoadMap();

	Status status;
	class Graphics* pGraphics;
	Game game;
	Map map;
};
