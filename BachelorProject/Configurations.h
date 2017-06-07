/*
Configurations.h
Created by Martin Dariush Hansen, 2017-03-04
*/

#pragma once

// Program
const bool PERFORMANCE_TESTS = true;
const bool UNIT_TESTS = false;
const bool WARNINGS = false;
const bool LIMIT_SPEED = false;
const int TIME_LOOP_RUNS = -1; // -1 = infinite

// Graphics
const bool RENDER_GRAPHICS = false;
const bool DRAW_GRAPH = false;

// Units
const int UNITS_TO_CREATE = 1000;
const int MAX_UNITS_ON_MAP = 1000;
const int UNIT_DESTINATIONS = 1;

// Terrain
const char MAP_NAME[] = "MapBlank32x32.png";
const bool DYNAMIC_TERRAIN = false;
const int TERRAIN_TIMER_MIN = 5;
const int TERRAIN_TIMER_MAX = 15;
const int DYNAMIC_TERRAIN_CLUSTER_RETRIES = 4;

// Path planning
const enum PathType {
	GRID,
	VISIBILITY_DECOMPOSED,
	VISIBILITY_FULL
};
const PathType GRAPH_TYPE = GRID;
const int VISIBILITY_SECTION_WIDTH = 8; // Must fit into map dimensions
const int VISIBILITY_SECTION_HEIGHT = VISIBILITY_SECTION_WIDTH;
