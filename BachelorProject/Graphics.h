/*
Graphics.h
Created by Martin Dariush Hansen, 2017-03-15
*/

#pragma once
#include "Map.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Graphics
{
public:
	Graphics();
	~Graphics();
	void Init(class Model* model, class Map* map);
	void RenderGraphics();
	bool getWindowOpen();
	sf::RenderWindow* getWindow();
	void setMapDimensions(unsigned int mapWidth0, unsigned int mapHeight0);
	void computeScaling();
	void generateBackgroundTexture();

private:
	class Model* pModel;
	class Map* pMap;

	sf::RenderWindow window;
	sf::Texture textureTile;
	sf::RenderTexture renderTextureBackground;
	sf::Sprite spriteBackground;
	sf::Sprite spriteTileOpen;
	sf::Sprite spriteTileClosed;
	sf::Sprite spriteTileUnknown;

	long double scaling;
	unsigned int mapWidth;
	unsigned int mapHeight;
};
