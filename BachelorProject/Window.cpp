// Window.cpp
// Created by Martin Dariush Hansen, 2017-03-12

#include "Constants.h"
#include "Window.h"
#include <SFML/Graphics.hpp>

Window::Window() {
}

Window::~Window() {
}

void Window::Init() {

	// Create window
	window.create(sf::VideoMode(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y), "Bachelor Project");
}

void Window::Refresh() {
	//window.clear(sf::Color::Black);

	window.display();
}
