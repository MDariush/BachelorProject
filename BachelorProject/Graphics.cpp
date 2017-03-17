/*
WindowManager.cpp
Created by Martin Dariush Hansen, 2017-03-15
*/

#include "Constants.h"
#include "Graphics.h"
#include "Model.h"
#include <iostream>
using namespace std;

Graphics::Graphics() : window(sf::VideoMode(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y), "Bachelor Project") {
}

Graphics::~Graphics() {
}

void Graphics::Init(Model* model) {
	pModel = model;

	textureTile.loadFromFile("Tile8x8.png");
	spriteTileOpen.setTexture(textureTile);
	spriteTileOpen.setColor(sf::Color(191, 191, 127));
	spriteTileClosed.setTexture(textureTile);
	spriteTileClosed.setColor(sf::Color(127, 63, 31));
	spriteTileUnknown.setTexture(textureTile);
	spriteTileUnknown.setColor(sf::Color(127, 127, 127));
}

void Graphics::RenderGraphics() {
	window.clear();

	if (pModel->getStatus() == Model::Status::IN_MAP) {
		window.draw(spriteBackground);
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
	spriteTileUnknown.setScale(sf::Vector2f(scaling / spriteTileUnknown.getTexture()->getSize().x, scaling / spriteTileUnknown.getTexture()->getSize().x));

	cout << "Sprites scaled.\n";
}

void Graphics::generateBackgroundTexture() {
	renderTextureBackground.create(mapWidth * scaling, mapHeight * scaling);

	renderTextureBackground.clear();

	for (signed int i = 0; i < mapWidth; i++) {
		for (signed int j = 0; j < mapHeight; j++) {
			if (pModel->map.getCellStatus(i, j) == Map::CellStatus::OPEN) {
				spriteTileOpen.setPosition(sf::Vector2f(i * scaling, j * scaling));
				renderTextureBackground.draw(spriteTileOpen);
			}
			else if (pModel->map.getCellStatus(i, j) == Map::CellStatus::CLOSED) {
				spriteTileClosed.setPosition(sf::Vector2f(i * scaling, j * scaling));
				renderTextureBackground.draw(spriteTileClosed);
			}
		}
	}

	spriteBackground.setTexture(renderTextureBackground.getTexture());

}
