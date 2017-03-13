// Timer_Test.cpp
// Created by Martin Dariush Hansen, 2017-03-04
/*
#include "catch.hpp"
#include "Timer.h"

TEST_CASE("Timer test with no iterans", "[timer]") {
	Window window;
	Timer timer;

	const signed int timeLoopRuns = 0;
	timer.Init(& window, timeLoopRuns);
	timer.TimeLoop(timer.getTimeLoopRuns());

	REQUIRE(timer.getTimeLoopRuns() == 0);
	REQUIRE(timer.getProgramSteps() == timeLoopRuns);
	REQUIRE(timer.getProgramTime() >= timeLoopRuns * STEP_DURATION);
}

TEST_CASE("Timer test with single iteration", "[timer]") {
	Window window;
	Timer timer;

	const signed int timeLoopRuns = 1;
	timer.Init(& window, timeLoopRuns);
	timer.TimeLoop(timer.getTimeLoopRuns());

	REQUIRE(timer.getTimeLoopRuns() == 0);
	REQUIRE(timer.getProgramSteps() == timeLoopRuns);
	REQUIRE(timer.getProgramTime() >= timeLoopRuns * STEP_DURATION);
}

TEST_CASE("Timer test with multiple iterans", "[timer]") {
	Window window;
	Timer timer;

	const signed int timeLoopRuns = 4;
	timer.Init(& window, timeLoopRuns);
	timer.TimeLoop(timer.getTimeLoopRuns());

	REQUIRE(timer.getTimeLoopRuns() == 0);
	REQUIRE(timer.getProgramSteps() == timeLoopRuns);
	REQUIRE(timer.getProgramTime() >= timeLoopRuns * STEP_DURATION);
}

TEST_CASE("Testing timer performance", "[timer]") {
	Window window;
	Timer timer;

	const signed int timeLoopRuns = 4;
	timer.Init(& window, timeLoopRuns);
	timer.TimeLoop(timer.getTimeLoopRuns());

	REQUIRE(timer.getProgramTime() <= timeLoopRuns * STEP_DURATION * 2);
}
*/