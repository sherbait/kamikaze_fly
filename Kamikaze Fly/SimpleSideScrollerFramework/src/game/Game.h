/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Game.h

	The class is the central object of the game
	application. It plays two important roles:

	1) It manages all major game objects (Game...), which
	essentially manage the game data and communicates with
	external technologies like graphics cards and input
	devices. The objects can be accessed via inlined public
	methods, i.e. getGraphics(), getInput(), getTimer(), etc.

	2) It runs the game loop, calling the appropriate
	methods in the other major objects as needed. The
	runGameLoop method iterates once per frame, invoking
	all necessary methods needed each frame to run the game.

	NOTE: I have deliberately avoided making this class 
	technology specific. I would like to be able to change
	the way the graphics are rendered (i.e. DirectX or OpenGL)
	if I want to without having to change this or most other
	classes.
*/

#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "src\gsm\state\GameState.h"

// FORWARD DECLARATIONS OF CLASSES USED BY GAME
class GameDataLoader;
class GameGraphics;
class GameInput;
class GameOS;
class GameTimer;
class GameStateManager;
class GameGUI;
class GameText;
class AudioManager;

static const float PI = 3.1415926535897932384626433f;

class Game
{
private:
	Game();
	~Game();
	Game(Game const&){};             // copy constructor is private
    Game& operator=(Game const&){};  // assignment operator is private

	// THE GameStateManager DEALS WITH ALL THE
	// GAME-PLAY AND GAME-WORLD STUFF, LIKE AI,
	// PHYSICS, AND GAME WORLD CONSTRUCTION. IT
	// ALSO KNOWS THE CURRENT GAME STATE
	GameStateManager	*gsm;

	// MANAGES ALL THE GUIS
	GameGUI				*gui;

	// MANAGES TEXT FOR RENDERING AND FILE I/O
	GameText			*text;

	// THESE GAME OBJECTS ALL REQUIRE CUSTOM DEFINITIONS, EITHER
	// BECAUSE THEY DEAL WITH PLATFORM-SPECIFIC THINGS, OR GAME-
	// SPECIFIC THINGS
	GameDataLoader		*dataLoader;
	GameGraphics		*graphics;
	GameInput			*input;
	GameOS				*os;
	GameTimer			*timer;
	AudioManager		*audio;

	// THIS WILL KEEP TRACK OF THE NAME OF THE LEVEL FILE
	// THAT IS CURRENTLY BEING USED
	wstring				currentLevelFileName;

	float waypointX;
	float waypointY;

public:
	static Game* getInstance();	

	float getWaypointX(){return waypointX;}
	float getWaypointY(){return waypointY;}

	// INLINED ACCESSOR METHODS - THE REAL THINGS
	GameStateManager*	getGSM()				{ return gsm;								}
	GameGUI*			getGUI()				{ return gui;								}
	GameText*			getText()				{ return text;								}

	// INLINED ACCESSOR METHODS - ACTUALLY SUB-CLASSES
	GameDataLoader*		getDataLoader()			{ return dataLoader;						}
	GameGraphics*		getGraphics()			{ return graphics;							}
	GameInput*			getInput()				{ return input;								}
	GameOS*				getOS()					{ return os;								}
	GameTimer*			getTimer()				{ return timer;								}
	AudioManager*		getAudioManager()		{ return audio;								}

	// AND FOR GETTING THE NAME OF THE FILE USED FOR LOADING THIS LEVEL
	wstring				getCurrentLevelFileName()	{ return currentLevelFileName;			}

	// INLINED MUTATOR METHODS
	void	setDataLoader(GameDataLoader *initDL) { dataLoader = initDL; }
	void	setCurrentLevelFileName(wstring initCurrentLevelFileName) { currentLevelFileName = initCurrentLevelFileName; }
	void	setWaypoint(float x, float y){waypointX=x; waypointY=y;}

	// METHODS DEFINED IN Game.cpp

	void	initPlatformPlugins(	GameGraphics *initGraphics, 
									GameInput *initInput, 
									GameOS *initOS,
									GameTimer *initTimer);
	void	processGameData();
	void	quitApp();
	void	quitGame();
	void	reloadAllDevices();
	void	runGameLoop();
	void	shutdown();
	void	startGame();
};