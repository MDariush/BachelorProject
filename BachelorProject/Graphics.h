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
	void Init(class Model* pModelArg);
	void RenderGraphics(int programStepsArg);
	bool getWindowOpen();
	sf::RenderWindow* getWindow();
	void ComputeScaling();
	void GenerateBackgroundTexture();
	void GenerateFogTexture();
	void GenerateGraphTexture();
	void UpdateEntireGraphTexture(int programStepsArg);
	void UpdateEntireFogTexture();
	void setMapDimensions(int mapWidthArg, int mapHeightArg);
	void setMapPtr(class Map* pMapArg);

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
	int drawnMapGeneration;
	int drawnFogGeneration;
	int drawnGraphGeneration;
	std::vector<class Player>* pPlayers;
	int* pTimeStep;
};
