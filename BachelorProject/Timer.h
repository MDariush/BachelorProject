// Timer.h
// Created by Martin Dariush Hansen, 2017-03-01

#pragma once
#include <chrono>
using namespace std::chrono;

class Timer {
public:
	Timer();
	~Timer();
	void Init();
	void TimeLoop();

private:
	const milliseconds stepDuration = milliseconds(5);
	
	steady_clock::time_point currentTime;
	steady_clock::time_point programStartTime;
	steady_clock::time_point previousStepTime;
	int programSteps;
};
