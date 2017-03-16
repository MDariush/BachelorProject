/*
Model.cpp
Created by Martin Dariush Hansen, 2017-03-14
*/

#include "Graphics.h"
#include "Map.h"
#include "Model.h"

Model::Model() {
}

Model::~Model() {
}

void Model::Init(Graphics* graphics) {
	pGraphics = graphics;

	status = Status::LOADING_MAP;
	map.Init("TestMap1.png");
}

void Model::Step() {
	if (status == Status::LOADING_MAP) {
		if (!map.getMapLoaded()) {
			map.LoadMapImage();
			pGraphics->computeScaling();
		}
		else {
			status = Status::IN_MAP;
		}
	}
}

Model::Status Model::getStatus() {
	return status;
}

void Model::LoadMap() {
}