/*
Constants.h
Created by Martin Dariush Hansen, 2017-03-04
*/

#pragma once

// Math
const double PI =   3.141592653589793238;
const double PI_2 = 6.283185307179586476;
const double PI_2_TO_DEG = 360 / PI_2;

// Screen resolution
const int TARGET_RESOLUTION_X = 1920;
const int TARGET_RESOLUTION_Y = 1080;
const int MIN_RESOLUTION_X = TARGET_RESOLUTION_X / 4;
const int MIN_RESOLUTION_Y = TARGET_RESOLUTION_Y / 4;
const int MAX_RESOLUTION_X = TARGET_RESOLUTION_X * 4;
const int MAX_RESOLUTION_Y = TARGET_RESOLUTION_Y * 4;
const int DEFAULT_RESOLUTION_X = 1000; // TARGET_RESOLUTION_X / 2;
const int DEFAULT_RESOLUTION_Y = 1000; // TARGET_RESOLUTION_Y / 2;

// Time
const int NANO = 1000000000;
const int STEPS_PER_SECOND = 200; // Should be higher than target fps
const int STEP_DURATION = NANO / STEPS_PER_SECOND;
const int TARGET_FPS = 60;
const int MIN_FPS = 15;
const int MAX_FPS = STEPS_PER_SECOND;