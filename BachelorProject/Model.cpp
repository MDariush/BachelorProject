/*
Model.cpp
Created by Martin Dariush Hansen, 2017-03-14
*/

#include "Game.h"
#include "Graphics.h"
#include "Model.h"
#include "Player.h"

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
				map.Init("TestMapRandom40x40.png");
				pGraphics->setMapDimensions(map.getWidth(), map.getHeight());
				pGraphics->ComputeScaling();
			}
			else if (!game.getInitialized()) {
				game.Init(&map);
				pGraphics->GenerateFogTexture();
				pGraphics->GenerateGraphTexture();
				pGraphics->setMapPtr(game.getPlayersPtr()->at(0).getVisionPtr()->getMapPtr());
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