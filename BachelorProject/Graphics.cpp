/*
WindowManager.cpp
Created by Martin Dariush Hansen, 2017-03-15
*/

#include "Constants.h"
#include "Graphics.h"
#include "Map.h"
#include "Model.h"
#include "Player.h"
#include "Unit.h"
#include <iostream>
using namespace std;

Graphics::Graphics() : window(sf::VideoMode(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y), "Bachelor Project") {
}

Graphics::~Graphics() {
}

void Graphics::Init(Model* pModel0, Map* pMap0, std::vector<class Player>* pPlayers0) {
	pModel = pModel0;
	pMap = pMap0;
	pPlayers = pPlayers0;

	shapeUnexploredPixel.setSize(sf::Vector2f(1, 1));
	shapeUnexploredPixel.setFillColor(sf::Color(0, 0, 0, 255));
	shapeExploredPixel.setSize(sf::Vector2f(1, 1));
	shapeExploredPixel.setFillColor(sf::Color(0, 0, 0, 127));

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
	if (pMap->getMapUpdated()) {
		GenerateBackgroundTexture();
		pMap->setMapUpdated(false);
	}

	if (pModel->getStatus() == Model::Status::IN_MAP) {
		window.draw(spriteBackground);

		// Draw units
		for (int i = 0; i < pModel->getGamePtr()->getPlayersPtr()->size() ; i++) {
			for (int j = 0; j < pModel->getGamePtr()->getPlayersPtr()->at(i).getUnitsPtr()->size(); j++) {
				spriteUnit.setPosition(sf::Vector2f(pModel->getGamePtr()->getPlayersPtr()->at(i).getUnitsPtr()->at(j).getX() * scaling, pModel->getGamePtr()->getPlayersPtr()->at(i).getUnitsPtr()->at(j).getY() * scaling));
				spriteUnit.setRotation(pModel->getGamePtr()->getPlayersPtr()->at(i).getUnitsPtr()->at(j).getOrientationDeg());
				window.draw(spriteUnit);
			}
		}

		// Draw fog
		UpdateEntireFogTexture();
		window.draw(spriteFog);

		// Draw graph
		UpdateEntireGraphTexture(); // TODO: Avoid recomputation
		window.draw(spriteGraph);

		// Draw paths
		for (int i = 0; i < pModel->getGamePtr()->getPlayersPtr()->size(); i++) {
			for (int j = 0; j < pModel->getGamePtr()->getPlayersPtr()->at(i).getUnitsPtr()->size(); j++) {
				stack<std::pair<double, double>> path = pModel->getGamePtr()->getPlayersPtr()->at(i).getUnitsPtr()->at(j).getCurrentCommand().path;
				if (path.size() > 1) {
					for (int k = 0; k < path.size() - 1; k) {
						int x1 = path.top().first * scaling;
						int y1 = path.top().second * scaling;
						path.pop();
						int x2 = path.top().first * scaling;
						int y2 = path.top().second * scaling;
						sf::Vertex line[] = {
							sf::Vertex(sf::Vector2f(x1, y1)),
							sf::Vertex(sf::Vector2f(x2, y2))
						};

						window.draw(line, 2, sf::Lines);
					}
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

void Graphics::setMapDimensions(int mapWidth0, int mapHeight0) {
	mapWidth = mapWidth0;
	mapHeight = mapHeight0;
}

void Graphics::ComputeScaling() {
	scaling = window.getSize().x / mapWidth;

	spriteTileOpen.setScale(sf::Vector2f(scaling / spriteTileOpen.getTexture()->getSize().x, scaling / spriteTileOpen.getTexture()->getSize().x));
	spriteTileClosed.setScale(sf::Vector2f(scaling / spriteTileClosed.getTexture()->getSize().x, scaling / spriteTileClosed.getTexture()->getSize().x));

	if (scaling > 1) {
		spriteUnit.setScale(sf::Vector2f(scaling / spriteTileClosed.getTexture()->getSize().x, scaling / spriteTileClosed.getTexture()->getSize().x));
	}

	cout << "Scaling set to: " << scaling << ".\n";
}

void Graphics::GenerateFogTexture() {
	renderTextureFog.create(mapWidth, mapHeight);
	renderTextureFog.clear(sf::Color(0, 0, 0, 255));

	renderTextureFog.display();
	spriteFog.setTexture(renderTextureFog.getTexture());
	spriteFog.scale(sf::Vector2f(scaling, scaling));
}

void Graphics::GenerateGraphTexture() {
	renderTextureGraph.create(mapWidth * scaling, mapHeight * scaling);
	renderTextureGraph.clear(sf::Color(0, 0, 0, 0));

	renderTextureGraph.display();
	spriteGraph.setTexture(renderTextureGraph.getTexture());
}

void Graphics::UpdateEntireFogTexture() {
	renderTextureFog.clear(sf::Color(0, 0, 0, 0));

	for (int i = 0; i < mapWidth; i++) {
		for (int j = 0; j < mapHeight; j++) {
			switch (pPlayers->at(0).getVisionArrayPtr()->at(i).at(j)) {
			case Vision::UNEXPLORED:
				shapeUnexploredPixel.setPosition(sf::Vector2f(i, j));
				renderTextureFog.draw(shapeUnexploredPixel);
				break;
			case Vision::EXPLORED:
				shapeExploredPixel.setPosition(sf::Vector2f(i, j));
				renderTextureFog.draw(shapeExploredPixel);
				break;
			default:
				break;
			}
		}
	}
	renderTextureFog.display();
	spriteFog.setTexture(renderTextureFog.getTexture());
}

void Graphics::UpdateEntireGraphTexture() {
	renderTextureGraph.clear(sf::Color(0, 0, 0, 0));

	for (int i = 0; i < pModel->getGamePtr()->getPlayersPtr()->size(); i++) {
		for (int j = 0; j < mapWidth; j++) {
			for (int k = 0; k < mapHeight; k++) {
				std::vector<std::vector<Pathfinder::Node>>* nodes = pModel->getGamePtr()->getPlayersPtr()->at(i).getPathfinderPtr()->getNodesPtr();
				
				int x1 = (j + 0.5) * scaling;
				int y1 = (k + 0.5) * scaling;

				//for (int l = 0; l < nodes->at(j).at(k).neighbors.size(); l++) {
				for (forward_list<pair<double, pair<int, int>>>::iterator it = nodes->at(j).at(k).neighbors.begin(); it != nodes->at(j).at(k).neighbors.end(); ) {

					//int x2 = (nodes->at(j).at(k).neighbors.at(l).second.first + 0.5) * scaling;
					//int y2 = (nodes->at(j).at(k).neighbors.at(l).second.second + 0.5) * scaling;
					int x2 = (it->second.first + 0.5) * scaling;
					int y2 = (it->second.second + 0.5) * scaling;

					sf::Vertex line[] = {
						sf::Vertex(sf::Vector2f(x1, y1), sf::Color(128, 255, 128, 128)),
						sf::Vertex(sf::Vector2f(x2, y2), sf::Color(128, 255, 128, 128))
					};

					renderTextureGraph.draw(line, 2, sf::Lines);

					++it;
				}
			}
		}
	}

	renderTextureGraph.display();
	spriteGraph.setTexture(renderTextureGraph.getTexture());
}

void Graphics::GenerateBackgroundTexture() {
	renderTextureBackground.create(mapWidth * scaling, mapHeight * scaling);
	renderTextureBackground.clear(sf::Color(0, 0, 0, 255));

	for (int i = 0; i < mapWidth; i++) {
		for (int j = 0; j < mapHeight; j++) {
			switch (pMap->getCellStatus(i, j)) {
			case Map::CellStatus::CLOSED:
				spriteTileClosed.setPosition(sf::Vector2f(i * scaling, j * scaling));
				renderTextureBackground.draw(spriteTileClosed);
				break;
			default:
				spriteTileOpen.setPosition(sf::Vector2f(i * scaling, j * scaling));
				renderTextureBackground.draw(spriteTileOpen);
				break;
			}
		}
	}
	renderTextureBackground.display();
	spriteBackground.setTexture(renderTextureBackground.getTexture());
}
