/*
Program.h
Created by Martin Dariush Hansen, 2017-03-01
*/

#pragma once
#include "Constants.h"
#include "Model.h"
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

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

	Model model;

	// Window
	sf::RenderWindow window;
	sf::CircleShape shape;

	// Time
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


