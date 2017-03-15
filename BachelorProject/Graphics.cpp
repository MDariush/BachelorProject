/*
WindowManager.cpp
Created by Martin Dariush Hansen, 2017-03-15
*/

#include "Constants.h"
#include "Graphics.h"

Graphics::Graphics() : window(sf::VideoMode(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y), "Bachelor Project") {
}

Graphics::~Graphics() {
}

void Graphics::Init() {

	// Load/define graphics
	shape.setRadius(32.f);
	shape.setPosition(240.f, 240.f);
	shape.setFillColor(sf::Color::Cyan);
}

void Graphics::RenderGraphics() {
	window.clear();

	window.draw(shape);

	window.display();
}

bool Graphics::getWindowOpen() {
	return window.isOpen();
}

sf::RenderWindow * Graphics::getWindow() {
	return &window;
}
