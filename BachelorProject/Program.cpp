// Program.cpp
// Created by Martin Dariush Hansen, 2017-03-13

#include "catch.hpp"
#include "Program.h"
#include <iostream>
using namespace std;

Program::Program() : window(sf::VideoMode(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y), "Bachelor Project") {
}

Program::~Program() {
}

void Program::Init(int timeLoopRuns0) {

	// Window setup
	//sf::RenderWindow window(sf::VideoMode(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y), "Bachelor Project");

	shape.setRadius(40.f);
	shape.setPosition(100.f, 100.f);
	shape.setFillColor(sf::Color::Cyan);

	// Time setup
	currentTime = high_resolution_clock::now();
	programStartTime = currentTime;
	previousStepTime = currentTime;
	programSteps = 0;
	programTime = 0;
	timeLoopRuns = timeLoopRuns0;

	TimeLoop(timeLoopRuns);
}

void Program::TimeLoop(int timeLoopRuns0) {
	timeLoopRuns = timeLoopRuns0;
	while (timeLoopRuns != 0 /*&& window.isOpen()*/) {
		ProcessEvents();
		ProgramStep();
		RenderGraphics();
		UpdateTime();
	}
}

void Program::ProcessEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
		}
	}
}

void Program::ProgramStep() {
}

void Program::RenderGraphics() {
	window.clear();

	window.draw(shape);

	window.display();
}

void Program::UpdateTime() {
	currentTime = high_resolution_clock::now();
	duration<double, std::milli> timeSincePreviousStep = currentTime - previousStepTime;

	if (timeSincePreviousStep >= stepDuration) {
		programSteps++;
		previousStepTime = currentTime;
		programTime += timeSincePreviousStep.count();
		if (timeLoopRuns > 0) {
			timeLoopRuns--;
		}
		cout << "Step " << programSteps << ", " << timeSincePreviousStep.count() << " ms.\n";
	}
}

int Program::getTimeLoopRuns() {
	return timeLoopRuns;
}

int Program::getProgramSteps() {
	return programSteps;
}

double Program::getProgramTime() {
	return programTime;
}