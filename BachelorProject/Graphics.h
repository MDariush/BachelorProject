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
	void Init();
	void RenderGraphics();
	bool getWindowOpen();
	sf::RenderWindow* getWindow();

private:
	sf::RenderWindow window;
	sf::CircleShape shape;
};

