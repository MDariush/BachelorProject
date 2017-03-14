/*
main.cpp
Created by Martin Dariush Hansen, 2017-03-01

External dependencies used in this project:
 - "Catch". For unit testing. catch.hpp only.
 - "LodePNG". For reading png-files. lodepng.cpp and lodepng.h only.
 - "SFML". For window and graphics. Installed.
*/

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "Configurations.h"
#include "Program.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;

int main(int argc, char* argv[]) {
	Program program;
	program.Init(TIME_LOOP_RUNS);

	cout << "\nPROGRAM ENDED";
	std::getchar();

	/*
	// Run unit tests
	int unitTestResult = 0;

	if (UNIT_TESTS_ENABLED == true) {
		int unitTestResult = Catch::Session().run(argc, argv);
	}

	// Run program
	Window window;
	window.Init();
	Timer timer;
	timer.Init(&window, -1);

	cout << "\nPROGRAM ENDED";
	std::getchar();

	return (unitTestResult < 0xff ? unitTestResult : 0xff);
	*/
	return 0;
}
