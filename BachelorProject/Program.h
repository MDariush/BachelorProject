// Program.h
// Created by Martin Dariush Hansen, 2017-03-01

#pragma once
#include "Constants.h"
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std::chrono;

class Program
{
public:
	Program();
	~Program();
	void Init(int timeLoopRuns0);
	int getTimeLoopRuns();
	unsigned int getProgramSteps();
	double getProgramTime();

private:
	void TimeLoop(int timeLoopRuns0);
	void ProcessEvents();
	void ProgramStep();
	void RenderGraphics();
	void UpdateSteps();
	void RecordFrameTime();
	bool ReadyForProgramStep();
	bool ReadyToRenderGraphics();

	// Window
	sf::RenderWindow window;
	sf::CircleShape shape;

	// Time
	steady_clock::time_point programStartTime;
	steady_clock::time_point currentTime;
	int timeLoopRuns;
	double programTime;
	nanoseconds stepDuration;
	nanoseconds frameDuration;
	steady_clock::time_point previousStepTime;
	steady_clock::time_point previousFrameTime;
	duration<double, std::milli> timeSincePreviousStep;
	duration<double, std::milli> timeSincePreviousFrame;
	unsigned int programSteps;
	unsigned int programFrames;
};
