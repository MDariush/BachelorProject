/*
Program.h
Created by Martin Dariush Hansen, 2017-03-01
*/

#pragma once
#include "Constants.h"
#include "Controls.h"
#include "Graphics.h"
#include "Model.h"
#include <chrono>

class Timer
{
public:
	Timer();
	~Timer();
	void Init(int timeLoopRuns0);
	int getTimeLoopRuns();
	unsigned int getProgramSteps();
	double getProgramTime();

private:
	void TimeLoop(int timeLoopRuns0);
	void UpdateSteps();
	void RecordFrameTime();
	bool ReadyForProgramStep();
	bool ReadyToRenderGraphics();

	Model model;
	Graphics graphics;
	Controls controls;

	std::chrono::steady_clock::time_point programStartTime;
	std::chrono::steady_clock::time_point currentTime;
	int timeLoopRuns;
	double programTime;
	std::chrono::nanoseconds stepDuration;
	std::chrono::nanoseconds frameDuration;
	std::chrono::steady_clock::time_point previousStepTime;
	std::chrono::steady_clock::time_point previousFrameTime;
	std::chrono::duration<double, std::milli> timeSincePreviousStep;
	std::chrono::duration<double, std::milli> timeSincePreviousFrame;
	unsigned int programSteps;
	unsigned int programFrames;
};
