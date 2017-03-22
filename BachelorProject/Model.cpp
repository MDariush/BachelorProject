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
	
}

void Model::Step() {
	switch (status) {
		case Model::LOADING_MAP:
			if (!map.getInitialized()) {
				map.Init("TestMapRandom100x100.png");
				pGraphics->setMapDimensions(map.getWidth(), map.getHeight());
				pGraphics->computeScaling();
				pGraphics->generateBackgroundTexture();
			}
			else if (!game.getInitialized()) {
				game.Init(&map);
			}
			else {
				status = Status::IN_MAP;
			}
			break;
		case Model::IN_MAP:
			game.Step();
			break;
		default:
			break;
	}
}

Model::Status Model::getStatus() {
	return status;
}

Game * Model::getGamePtr() {
	return &game;
}

Map * Model::getMapPtr() {
	return &map;
}

void Model::LoadMap() {
}