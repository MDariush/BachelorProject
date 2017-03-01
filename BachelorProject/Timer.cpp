// Timer.cpp
// Created by Martin Dariush Hansen, 2017-03-01

#include "Timer.h"
#include <iostream>
using namespace std;

Timer::Timer() {
}

Timer::~Timer() {
}

void Timer::Init() {
	currentTime = high_resolution_clock::now();
	programStartTime = currentTime;
	previousStepTime = currentTime;
	programSteps = 0;

	TimeLoop();
}

void Timer::TimeLoop() {
	while (true) {

		// Program step


		// Timing
		currentTime = high_resolution_clock::now();
		std::chrono::duration<double, std::milli> timeSincePreviousStep = currentTime - previousStepTime;

		if (timeSincePreviousStep >= stepDuration) {
			programSteps++;
			previousStepTime = currentTime;
			cout << "Step number: " << programSteps << ".   Step length [ms]: " << timeSincePreviousStep.count() << ".\n";
		}
	}
}
