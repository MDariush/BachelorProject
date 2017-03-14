/*
Model.cpp
Created by Martin Dariush Hansen, 2017-03-14
*/

#include "Model.h"

Model::Model() {
}

Model::~Model() {
}

void Model::Init() {
	status = Status::LOADING_MAP;
	
}

void Model::Step() {
	/*if (status == Status::LOADING_MAP) {
		if (!map.getMapLoaded()) {
			map.LoadMapImage();
		}
		else {
			status = Status::IN_MAP;
		}
	}*/
}

void Model::LoadMap() {
}