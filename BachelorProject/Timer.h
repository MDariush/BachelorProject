// Timer.h
// Created by Martin Dariush Hansen, 2017-03-01

#pragma once
#include "Constants.h"
#include "Window.h"
#include <chrono>
using namespace std::chrono;

class Timer {
public:
	Timer();
	~Timer();
	void Init(const Window & Window0, int timeLoopRuns0);
	void TimeLoop(int timeLoopRuns0);
	int getTimeLoopRuns();
	int getProgramSteps();
	double getProgramTime();

private:
	const milliseconds stepDuration = milliseconds(STEP_DURATION);

	steady_clock::time_point currentTime;
	steady_clock::time_point programStartTime;
	steady_clock::time_point previousStepTime;
	int timeLoopRuns;
	unsigned int programSteps;
	double programTime;
};
