// main.cpp
// Created by Martin Dariush Hansen, 2017-03-01

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "Timer.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	
	// Run unit tests
	const bool UNIT_TESTS_ENABLED = true;
	int unitTestResult = 0;

	if (UNIT_TESTS_ENABLED == true) {
		int unitTestResult = Catch::Session().run(argc, argv);
	}

	// Run program
	Timer Timer;
	Timer.Init(-1);

	cout << "\nPROGRAM ENDED";
	std::getchar();

	return (unitTestResult < 0xff ? unitTestResult : 0xff);
}

