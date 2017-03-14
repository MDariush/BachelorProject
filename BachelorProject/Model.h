/*
Model.h
Created by Martin Dariush Hansen, 2017-03-14
*/

#pragma once
#include "Map.h"

class Model
{
public:
	Model();
	~Model();

	void Init();
	void Step();

private:
	enum Status { LOADING_MAP, IN_MAP };
	
	void LoadMap();

	Status status;
	Map map;
};
