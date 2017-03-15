/*
Controls.h
Created by Martin Dariush Hansen, 2017-03-15
*/

#include "Controls.h"

Controls::Controls() {
}

Controls::~Controls() {
}

void Controls::Init() {

}

void Controls::ProcessEvents(sf::RenderWindow * renderWindow) {

	sf::Event event;
	while (renderWindow->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			renderWindow->close();
			break;
		}
	}
}
