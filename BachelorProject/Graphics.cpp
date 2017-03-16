/*
WindowManager.cpp
Created by Martin Dariush Hansen, 2017-03-15
*/

#include "Constants.h"
#include "Graphics.h"
#include "Model.h"

Graphics::Graphics() : window(sf::VideoMode(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y), "Bachelor Project") {
}

Graphics::~Graphics() {
}

void Graphics::Init(Model* model) {
	pModel = model;

	textureTile.loadFromFile("Tile8x8.png");
	spriteTileOpen.setTexture(textureTile);
	spriteTileOpen.setColor(sf::Color(191, 191, 191));
	spriteTileClosed.setTexture(textureTile);
	spriteTileClosed.setColor(sf::Color(47, 47, 47));
	spriteTileUnknown.setTexture(textureTile);
	spriteTileUnknown.setColor(sf::Color(95, 95, 95));
}

void Graphics::RenderGraphics() {
	window.clear();

	if (pModel->getStatus() == Model::Status::IN_MAP) {
		for (int i = 0; i < pModel->map.getWidth(); i++) {
			for (int j = 0; j < pModel->map.getHeight(); j++) {
				if (pModel->map.getCellStatusArray()[i][j] == Map::CellStatus::OPEN) {
					spriteTileOpen.setPosition(sf::Vector2f(i * scaling, j * scaling));
					window.draw(spriteTileOpen);
				}
				else if (pModel->map.getCellStatusArray()[i][j] == Map::CellStatus::CLOSED) {
					spriteTileClosed.setPosition(sf::Vector2f(i * scaling, j * scaling));
					window.draw(spriteTileClosed);
				}
			}
		}
	}
	
	window.display();
}

bool Graphics::getWindowOpen() {
	return window.isOpen();
}

sf::RenderWindow * Graphics::getWindow() {
	return &window;
}

void Graphics::computeScaling() {
	scaling = window.getSize().x / pModel->map.getWidth();

	spriteTileOpen.setScale(sf::Vector2f(scaling / spriteTileOpen.getTexture()->getSize().x, scaling / spriteTileOpen.getTexture()->getSize().x));
	spriteTileClosed.setScale(sf::Vector2f(scaling / spriteTileClosed.getTexture()->getSize().x, scaling / spriteTileClosed.getTexture()->getSize().x));
	spriteTileUnknown.setScale(sf::Vector2f(scaling / spriteTileUnknown.getTexture()->getSize().x, scaling / spriteTileUnknown.getTexture()->getSize().x));
}
