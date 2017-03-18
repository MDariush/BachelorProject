/*
WindowManager.cpp
Created by Martin Dariush Hansen, 2017-03-15
*/

#include "Constants.h"
#include "Graphics.h"
#include "Map.h"
#include "Model.h"
#include "Unit.h"
#include <iostream>
using namespace std;

Graphics::Graphics() : window(sf::VideoMode(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y), "Bachelor Project") {
}

Graphics::~Graphics() {
}

void Graphics::Init(Model* pModel0, Map* pMap0, std::vector<class Unit>* pUnits0) {
	pModel = pModel0;
	pMap = pMap0;
	pUnits = pUnits0;

	textureTile.loadFromFile("Tile10x10.png");
	spriteTileOpen.setTexture(textureTile);
	spriteTileOpen.setColor(sf::Color(191, 191, 127));
	spriteTileClosed.setTexture(textureTile);
	spriteTileClosed.setColor(sf::Color(127, 63, 31));

	textureUnit.loadFromFile("Robot10x10.png");
	spriteUnit.setTexture(textureUnit);
	spriteUnit.setColor(sf::Color(255, 255, 255));
	spriteUnit.setOrigin(spriteUnit.getTexture()->getSize().x / 2, spriteUnit.getTexture()->getSize().y / 2);
}

void Graphics::RenderGraphics() {
	window.clear();

	// Draw terrain
	if (pModel->getStatus() == Model::Status::IN_MAP) {
		window.draw(spriteBackground);

		// Draw units
		for (int i = 0; i < pUnits->size(); i++) {
			spriteUnit.setPosition(sf::Vector2f(pUnits->at(i).getX() * scaling, pUnits->at(i).getY() * scaling));
			window.draw(spriteUnit);
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

void Graphics::setMapDimensions(unsigned int mapWidth0, unsigned int mapHeight0) {
	mapWidth = mapWidth0;
	mapHeight = mapHeight0;
}

void Graphics::computeScaling() {
	scaling = window.getSize().x / mapWidth;

	spriteTileOpen.setScale(sf::Vector2f(scaling / spriteTileOpen.getTexture()->getSize().x, scaling / spriteTileOpen.getTexture()->getSize().x));
	spriteTileClosed.setScale(sf::Vector2f(scaling / spriteTileClosed.getTexture()->getSize().x, scaling / spriteTileClosed.getTexture()->getSize().x));

	if (scaling > 1) {
		spriteUnit.setScale(sf::Vector2f(scaling / spriteTileClosed.getTexture()->getSize().x, scaling / spriteTileClosed.getTexture()->getSize().x));
	}

	cout << "Scaling set to: " << scaling << ".\n";
}

void Graphics::generateBackgroundTexture() {
	renderTextureBackground.create(mapWidth * scaling, mapHeight * scaling);

	renderTextureBackground.clear();

	for (signed int i = 0; i < mapWidth; i++) {
		for (signed int j = 0; j < mapHeight; j++) {
			if (pMap->getCellStatus(i, j) == Map::CellStatus::OPEN) {
				spriteTileOpen.setPosition(sf::Vector2f(i * scaling, j * scaling));
				renderTextureBackground.draw(spriteTileOpen);
			}
			else if (pMap->getCellStatus(i, j) == Map::CellStatus::CLOSED) {
				spriteTileClosed.setPosition(sf::Vector2f(i * scaling, j * scaling));
				renderTextureBackground.draw(spriteTileClosed);
			}
		}
	}

	renderTextureBackground.display();

	spriteBackground.setTexture(renderTextureBackground.getTexture());

}
