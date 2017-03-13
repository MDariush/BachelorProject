// Constants.h
// Created by Martin Dariush Hansen, 2017-03-12

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Window {
public:
	Window();
	~Window();
	static void Init();
	void RenderingThread();

private:
	//sf::Window window;
	sf::RenderWindow window;
};
