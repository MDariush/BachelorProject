// Constants.h
// Created by Martin Dariush Hansen, 2017-03-12

#pragma once
#include <SFML/Window.hpp>

class Window {
public:
	Window();
	~Window();
	void Init();

	void Refresh();

private:
	sf::Window window;
};
