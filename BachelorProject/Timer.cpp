// Timer.cpp
// Created by Martin Dariush Hansen, 2017-03-01

#include "catch.hpp"
#include "Timer.h"
#include "Window.h"
#include <iostream>
using namespace std;

Timer::Timer() {
}

Timer::~Timer() {
}

void Timer::Init(Window* window0, int timeLoopRuns0) {
	currentTime = high_resolution_clock::now();
	programStartTime = currentTime;
	previousStepTime = currentTime;
	programSteps = 0;
	programTime = 0;
	timeLoopRuns = timeLoopRuns0;
	pWindow = window0;

	TimeLoop(timeLoopRuns);
}

void Timer::TimeLoop(int timeLoopRuns0) {
	timeLoopRuns = timeLoopRuns0;

	while (timeLoopRuns != 0) {

		// Program step


		// Draw graphics
		pWindow->RenderingThread();

		// Timing
		currentTime = high_resolution_clock::now();
		duration<double, std::milli> timeSincePreviousStep = currentTime - previousStepTime;

		if (timeSincePreviousStep >= stepDuration) {
			programSteps++;
			previousStepTime = currentTime;
			programTime += timeSincePreviousStep.count();
			if (timeLoopRuns > 0) {
				timeLoopRuns--;
			}
			//cout << "Step " << programSteps << ", " << timeSincePreviousStep.count() << " ms.\n";
		}
	}
}

int Timer::getTimeLoopRuns() {
	return timeLoopRuns;
}

int Timer::getProgramSteps() {
	return programSteps;
}

double Timer::getProgramTime() {
	return programTime;
}
