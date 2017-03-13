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
	sf::RenderWindow window(sf::VideoMode(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y), "Bachelor Project");

	// Start rendering thread
	//sf::Thread thread(&RenderingThread, &window);
	//thread.launch();

}

void Window::RenderingThread() {

	// Window events
	if (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	// Redraw window
	window.clear(sf::Color::Black);

	window.display();
}
