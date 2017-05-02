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
	void Init(class Model* pModel0, class Map* pMap0, std::vector<class Player>* pPlayers0);
	void RenderGraphics();
	bool getWindowOpen();
	sf::RenderWindow* getWindow();
	void setMapDimensions(int mapWidth0, int mapHeight0);
	void ComputeScaling();
	void GenerateBackgroundTexture();
	void GenerateFogTexture();
	void GenerateGraphTexture();
	void UpdateEntireGraphTexture();
	void UpdateEntireFogTexture();

private:
	class Model* pModel;
	class Map* pMap;

	sf::RectangleShape shapeUnexploredPixel;
	sf::RectangleShape shapeExploredPixel;
	sf::RenderWindow window;
	sf::RenderTexture renderTextureBackground;
	sf::RenderTexture renderTextureFog;
	sf::RenderTexture renderTextureGraph;
	sf::Sprite spriteBackground;
	sf::Sprite spriteFog;
	sf::Sprite spriteGraph;
	sf::Sprite spriteTileOpen;
	sf::Sprite spriteTileClosed;
	sf::Sprite spriteUnit;
	sf::Texture textureTile;
	sf::Texture textureUnit;

	double scaling;
	int mapWidth;
	int mapHeight;
	std::vector<class Player>* pPlayers;
};
