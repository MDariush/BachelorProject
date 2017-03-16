/*
WindowManager.cpp
Created by Martin Dariush Hansen, 2017-03-15
*/

#include "Constants.h"
#include "Model.h"
#include "Graphics.h"

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
		float tileSize = float(window.getSize().x / pModel->map.getWidth());

		for (int i = 0; i < pModel->map.getWidth(); i++) {
			for (int j = 0; j < pModel->map.getHeight(); j++) {
				if (pModel->map.getCellStatusArray()[i][j] == Map::CellStatus::OPEN) {
					spriteTileOpen.setPosition(sf::Vector2f(i * tileSize, j * tileSize));
					window.draw(spriteTileOpen);
				}
				else if (pModel->map.getCellStatusArray()[i][j] == Map::CellStatus::CLOSED) {
					spriteTileClosed.setPosition(sf::Vector2f(i * tileSize, j * tileSize));
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
