/*
Graphics.h
Created by Martin Dariush Hansen, 2017-03-15
*/

#pragma once
#include "Model.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Graphics
{
public:
	Graphics();
	~Graphics();
	void Init(Model* model);
	void RenderGraphics();
	bool getWindowOpen();
	sf::RenderWindow* getWindow();

private:
	sf::RenderWindow window;
	sf::Texture textureTile;
	sf::Sprite spriteTileOpen;
	sf::Sprite spriteTileClosed;
	sf::Sprite spriteTileUnknown;
	Model* pModel;
};
