/*
Model.cpp
Created by Martin Dariush Hansen, 2017-03-14
*/

#include "Game.h"
#include "Graphics.h"
#include "Model.h"

Model::Model() {
}

Model::~Model() {
}

void Model::Init(Graphics* graphics) {
	pGraphics = graphics;
	status = Status::LOADING_MAP;
	game.Init(&map);
	map.Init("TestMapRandom100x100.png");
}

void Model::Step() {
	if (status == Status::LOADING_MAP) {
		if (!map.getMapLoaded()) {
			map.LoadMapImage();
			pGraphics->setMapDimensions(map.getWidth(), map.getHeight());
			pGraphics->computeScaling();
			pGraphics->generateBackgroundTexture();
		}
		else {
			status = Status::IN_MAP;
			game.Step();
		}
	}
}

Model::Status Model::getStatus() {
	return status;
}

Map * Model::getMapPtr() {
	return &map;
}

void Model::LoadMap() {
}