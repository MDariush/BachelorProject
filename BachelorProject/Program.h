// Program.h
// Created by Martin Dariush Hansen, 2017-03-13

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
	void TimeLoop(int timeLoopRuns0);
	void ProcessEvents();
	void ProgramStep();
	void RenderGraphics();
	void UpdateTime();
	int getTimeLoopRuns();
	int getProgramSteps();
	double getProgramTime();

private:

	// Window
	sf::RenderWindow window;
	sf::CircleShape shape;

	// Time
	const milliseconds stepDuration = milliseconds(STEP_DURATION);
	steady_clock::time_point currentTime;
	steady_clock::time_point programStartTime;
	steady_clock::time_point previousStepTime;
	signed int timeLoopRuns;
	unsigned int programSteps;
	double programTime;
};
