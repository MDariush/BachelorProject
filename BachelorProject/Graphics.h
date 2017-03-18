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
	void Init(class Model* pModel0, class Map* pMap0, std::vector<class Unit>* pUnits0);
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
	sf::Texture textureUnit;
	sf::RenderTexture renderTextureBackground;
	sf::Sprite spriteBackground;
	sf::Sprite spriteTileOpen;
	sf::Sprite spriteTileClosed;
	sf::Sprite spriteUnit;

	long double scaling;
	unsigned int mapWidth;
	unsigned int mapHeight;
	std::vector<class Unit>* pUnits;
};
