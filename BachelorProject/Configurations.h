/*
Configurations.h
Created by Martin Dariush Hansen, 2017-03-04
*/

#pragma once

const bool UNIT_TESTS = true;
const bool WARNINGS = true;
const int TIME_LOOP_RUNS = -1; // -1 = infinite

// Graphics
const int FOG_GRAPHICS = 1;

// Units
const int UNITS_TO_CREATE = 1000;
const int MAX_UNITS_ON_MAP = 1;
const int UNIT_DESTINATIONS = 1000;

// Terrain
const bool DYNAMIC_TERRAIN = false;
const int TERRAIN_TIMER_MIN = 10;
const int TERRAIN_TIMER_MAX = 40;
const int DYNAMIC_TERRAIN_CLUSTER_RETRIES = 4;

// Path planning
const enum PathType {
	GRID,
	VISIBILITY_DECOMPOSED,
	VISIBILITY_FULL
};
const PathType GRAPH_TYPE = GRID;
const int VISIBILITY_SECTION_WIDTH = 10;
const int VISIBILITY_SECTION_HEIGHT = VISIBILITY_SECTION_WIDTH;
