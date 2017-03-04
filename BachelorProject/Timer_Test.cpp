// Timer_Test.cpp
// Created by Martin Dariush Hansen, 2017-03-04

#include "catch.hpp"
#include "Timer.h"

TEST_CASE("Timer test with no iterans", "[timer]") {
	Timer timer;

	const signed int timeLoopRuns = 0;
	timer.Init(timeLoopRuns);
	timer.TimeLoop(timer.getTimeLoopRuns());

	REQUIRE(timer.getTimeLoopRuns() == 0);
	REQUIRE(timer.getProgramSteps() == timeLoopRuns);
	REQUIRE(timer.getProgramTime() >= timeLoopRuns * STEP_DURATION);
}

TEST_CASE("Timer test with single iteration", "[timer]") {
	Timer timer;

	const signed int timeLoopRuns = 1;
	timer.Init(timeLoopRuns);
	timer.TimeLoop(timer.getTimeLoopRuns());

	REQUIRE(timer.getTimeLoopRuns() == 0);
	REQUIRE(timer.getProgramSteps() == timeLoopRuns);
	REQUIRE(timer.getProgramTime() >= timeLoopRuns * STEP_DURATION);
}

TEST_CASE("Timer test with multiple iterans", "[timer]") {
	Timer timer;

	const signed int timeLoopRuns = 4;
	timer.Init(timeLoopRuns);
	timer.TimeLoop(timer.getTimeLoopRuns());

	REQUIRE(timer.getTimeLoopRuns() == 0);
	REQUIRE(timer.getProgramSteps() == timeLoopRuns);
	REQUIRE(timer.getProgramTime() >= timeLoopRuns * STEP_DURATION);
}

TEST_CASE("Testing timer performance", "[timer]") {
	Timer timer;

	const signed int timeLoopRuns = 4;
	timer.Init(timeLoopRuns);
	timer.TimeLoop(timer.getTimeLoopRuns());

	REQUIRE(timer.getProgramTime() <= timeLoopRuns * STEP_DURATION * 2);
}
