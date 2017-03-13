// Program.cpp
// Created by Martin Dariush Hansen, 2017-03-01

#include "catch.hpp"
#include "Program.h"
#include <iostream>
using namespace std;

Program::Program() : window(sf::VideoMode(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y), "Bachelor Project") {
}

Program::~Program() {
}

void Program::Init(int timeLoopRuns0) {

	// Load/define graphics
	shape.setRadius(32.f);
	shape.setPosition(100.f, 100.f);
	shape.setFillColor(sf::Color::Cyan);

	// Time setup
	currentTime = high_resolution_clock::now();
	stepDuration = nanoseconds(STEP_DURATION);
	frameDuration = nanoseconds(NANO / TARGET_FPS);
	programStartTime = currentTime;
	previousStepTime = currentTime;
	previousFrameTime = currentTime;
	programSteps = 0;
	programFrames = 0;
	programTime = 0;
	timeLoopRuns = timeLoopRuns0;

	TimeLoop(timeLoopRuns);
}

void Program::TimeLoop(int timeLoopRuns0) {
	timeLoopRuns = timeLoopRuns0;
	while (timeLoopRuns != 0 && window.isOpen()) {

		// Events (e.g. mouse press)
		ProcessEvents();

		// Program step
		currentTime = high_resolution_clock::now();
		if (ReadyForProgramStep()) {
			ProgramStep();
			UpdateSteps();
		}

		// Render graphics
		currentTime = high_resolution_clock::now();
		if (ReadyToRenderGraphics()) {
			RenderGraphics();
			RecordFrameTime();
		}
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

	shape.setPosition(programSteps*10 % (960+32) - 32.f, 100.f);
	shape.setFillColor(sf::Color::Cyan);

	window.draw(shape);

	window.display();
}

void Program::UpdateSteps() {
	programSteps++;
	previousStepTime = currentTime;
	programTime += timeSincePreviousStep.count();
	if (timeLoopRuns > 0) {
		timeLoopRuns--;
	}
	//cout << "Step " << programSteps << ", " << timeSincePreviousStep.count() << " ms.\n";
}

void Program::RecordFrameTime() {
	programFrames++;
	previousFrameTime = currentTime;
	//cout << "Frame " << programFrames << ", " << timeSincePreviousFrame.count() << " ms.\n";
}

bool Program::ReadyForProgramStep() {
	timeSincePreviousStep = currentTime - previousStepTime;
	return timeSincePreviousStep >= stepDuration;
}

bool Program::ReadyToRenderGraphics() {
	timeSincePreviousFrame = currentTime - previousFrameTime;
	return timeSincePreviousFrame >= frameDuration;
}

int Program::getTimeLoopRuns() {
	return timeLoopRuns;
}

unsigned int Program::getProgramSteps() {
	return programSteps;
}

double Program::getProgramTime() {
	return programTime;
}
