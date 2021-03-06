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

	void Init(class Graphics* graphics, class Timer* pTimerArg);
	void Step();
	Status getStatus();
	Game* getGamePtr();
	Map* getMapPtr();

private:
	void LoadMap();

	class Graphics* pGraphics;
	class Timer* pTimer;

	Status status;
	Game game;
	Map map;
};
