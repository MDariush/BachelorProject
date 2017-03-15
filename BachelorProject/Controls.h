/*
Controls.h
Created by Martin Dariush Hansen, 2017-03-15
*/

#pragma once
#include "Graphics.h"

class Controls
{
public:
	Controls();
	~Controls();
	void Init();
	void ProcessEvents(sf::RenderWindow* renderWindow);
};

