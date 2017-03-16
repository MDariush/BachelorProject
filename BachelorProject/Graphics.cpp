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

	imageTile.loadFromFile("Tile8x8.png");
	textureTile.loadFromImage(imageTile);
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
		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				if (pModel->map.getCellStatus(i, j) == Map::CellStatus::OPEN) {
				//if (cellStatusArray[i][j] == Map::CellStatus::OPEN) {
					spriteTileOpen.setPosition(sf::Vector2f(i * scaling, j * scaling));
					window.draw(spriteTileOpen);
				}
				else if (pModel->map.getCellStatus(i, j) == Map::CellStatus::CLOSED) {
				//else if (cellStatusArray[i][j] == Map::CellStatus::CLOSED) {
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

	cout << "Sprites scaled.\n";
}

void Graphics::setMapDimensions(unsigned int mapWidth0, unsigned int mapHeight0) {
	mapWidth = mapWidth0;
	mapHeight = mapHeight0;
}

void Graphics::setMapCellStatusArray(std::vector<std::vector<Map::CellStatus>> cellStatusArray0) {
	cellStatusArray = cellStatusArray0;
}
