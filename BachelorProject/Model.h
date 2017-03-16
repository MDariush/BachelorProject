/*
Model.h
Created by Martin Dariush Hansen, 2017-03-14
*/

#pragma once
#include "Map.h"

class Model
{
public:
	enum Status { LOADING_MAP, IN_MAP };

	Model();
	~Model();

	void Init();
	void Step();

	Status getStatus();

	Map map;

private:	
	void LoadMap();

	Status status;
};
