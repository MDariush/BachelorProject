/*
Graphics.h
Created by Martin Dariush Hansen, 2017-03-15
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Graphics
{
public:
	Graphics();
	~Graphics();
	void Init(class Model* model);
	void RenderGraphics();
	bool getWindowOpen();
	sf::RenderWindow* getWindow();
	void computeScaling();

private:
	class Model* pModel;

	sf::RenderWindow window;
	sf::Texture textureTile;
	sf::Sprite spriteTileOpen;
	sf::Sprite spriteTileClosed;
	sf::Sprite spriteTileUnknown;

	float scaling;
};
