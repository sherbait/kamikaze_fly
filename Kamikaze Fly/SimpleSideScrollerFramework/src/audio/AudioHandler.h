/*
	Author: Dinia Gepte
			Stony Brook University
			Computer Science Department
*/

#pragma once
#include "stdafx.h"

class Game;

class AudioHandler
{
	protected:
	// INLINED CONSTRUCTOR
	AudioHandler(){}

	 // INLINED DESTRUCTOR
	~AudioHandler(){}

public:

	// METHOD DEFINED IN GAME-SPECIFIC CHILD CLASSES
	virtual void handleGameEvents(Game *game)=0;
	virtual void handleButtonEvents(Game *game)=0;
};