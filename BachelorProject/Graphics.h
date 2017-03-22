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
	void Init(class Model* pModel0, class Map* pMap0, std::vector<class Player>* pPlayers0, std::vector<class Unit>* pUnits0);
	void RenderGraphics();
	bool getWindowOpen();
	sf::RenderWindow* getWindow();
	void setMapDimensions(unsigned int mapWidth0, unsigned int mapHeight0);
	void ComputeScaling();
	void GenerateBackgroundTexture();
	void GenerateFogTexture();
	void UpdateEntireFogTexture();

private:
	class Model* pModel;
	class Map* pMap;

	sf::RectangleShape shapeVisionPixel;
	sf::RenderWindow window;
	sf::RenderTexture renderTextureBackground;
	sf::RenderTexture renderTextureFog;
	sf::Sprite spriteBackground;
	sf::Sprite spriteFog;
	sf::Sprite spriteTileOpen;
	sf::Sprite spriteTileClosed;
	sf::Sprite spriteUnit;
	sf::Texture textureTile;
	sf::Texture textureUnit;

	long double scaling;
	unsigned int mapWidth;
	unsigned int mapHeight;
	std::vector<class Player>* pPlayers;
	std::vector<class Unit>* pUnits;
};
