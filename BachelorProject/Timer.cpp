/*
Program.cpp
Created by Martin Dariush Hansen, 2017-03-01
*/

#include "catch.hpp"
#include "Timer.h"
#include <iostream>
using namespace std;
using namespace std::chrono;

Timer::Timer() {
}

Timer::~Timer() {
}

void Timer::Init(int timeLoopRuns0) {

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

	// Create objects
	model.Init(&graphics);
	graphics.Init(&model, model.getMapPtr(), model.getGamePtr()->getPlayersPtr());
	controls.Init();

	// Start timeloop
	TimeLoop(timeLoopRuns);
}

void Timer::TimeLoop(int timeLoopRuns0) {
	timeLoopRuns = timeLoopRuns0;
	while (timeLoopRuns != 0 && graphics.getWindowOpen()) {

		// Events (e.g. mouse press)
		controls.ProcessEvents(graphics.getWindow());

		// Program step
		currentTime = high_resolution_clock::now();
		if (ReadyForProgramStep()) {
			model.Step();
			UpdateSteps();
		}

		// Render graphics
		currentTime = high_resolution_clock::now();
		if (ReadyToRenderGraphics()) {
			graphics.RenderGraphics();
			RecordFrameTime();
		}
	}
}

void Timer::UpdateSteps() {
	programSteps++;
	previousStepTime = currentTime;
	programTime += timeSincePreviousStep.count();
	if (timeLoopRuns > 0) {
		timeLoopRuns--;
	}
	//cout << "Step " << programSteps << ", " << timeSincePreviousStep.count() << " ms.\n";
}

void Timer::RecordFrameTime() {
	programFrames++;
	previousFrameTime = currentTime;
	//cout << "Frame " << programFrames << ", " << timeSincePreviousFrame.count() << " ms.\n";
}

bool Timer::ReadyForProgramStep() {
	timeSincePreviousStep = currentTime - previousStepTime;
	return timeSincePreviousStep >= stepDuration;
}

bool Timer::ReadyToRenderGraphics() {
	timeSincePreviousFrame = currentTime - previousFrameTime;
	return timeSincePreviousFrame >= frameDuration;
}

int Timer::getTimeLoopRuns() {
	return timeLoopRuns;
}

unsigned int Timer::getProgramSteps() {
	return programSteps;
}

double Timer::getProgramTime() {
	return programTime;
}
