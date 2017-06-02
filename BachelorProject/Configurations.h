/*
Configurations.h
Created by Martin Dariush Hansen, 2017-03-04
*/

#pragma once

const bool UNIT_TESTS = true;
const bool WARNINGS = true;
const int TIME_LOOP_RUNS = -1; // -1 = infinite

// Units
const int UNITS_TO_CREATE = 1000;
const int MAX_UNITS_ON_MAP = 1;
const int UNIT_DESTINATIONS = 1000;

// Terrain
const char MAP_NAME[] = "TestMapRandom40x40.png";
const bool DYNAMIC_TERRAIN = true;
const int TERRAIN_TIMER_MIN = 5;
const int TERRAIN_TIMER_MAX = 15;
const int DYNAMIC_TERRAIN_CLUSTER_RETRIES = 4;

// Path planning
const enum PathType {
	GRID,
	VISIBILITY_DECOMPOSED,
	VISIBILITY_FULL
};
const PathType GRAPH_TYPE = VISIBILITY_FULL;
const int VISIBILITY_SECTION_WIDTH = 5; // Must fit into map dimensions
const int VISIBILITY_SECTION_HEIGHT = VISIBILITY_SECTION_WIDTH;
