/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutDataLoader.h

	This class provides a custom importer for the WalkaboutGame to import
	game data, gui data, and world (i.e. level) data.
*/
#pragma once
#include "stdafx.h"
#include "src\dataLoader\GameDataLoader.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"
#include "src\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"

class WalkaboutDataLoader : public GameDataLoader
{
private:
	// WE NEED THESE GUYS TO INIT OUR WINDOWS WINDOW
	HINSTANCE hInstance;
	int nCmdShow;

public:
	WalkaboutDataLoader()	{}
	~WalkaboutDataLoader()	{}

	// INLINED MUTATOR METHOD
	void initWinHandle(HINSTANCE initHInstance, int initNCmdShow)
	{
		hInstance = initHInstance;
		nCmdShow = initNCmdShow;
	}


	// THESE SHOULD BE LOADING DATA FROM FILES. THEY
	// ARE ALL DEFINED IN WalkaboutDataLoader.cpp
	void loadGame(Game *game, wstring gameInitFile);
	void loadGUI(Game *game, wstring guiInitFile);
	void loadWorld(Game *game, wstring levelInitFile);

	// THESE ARE HARD-CODED EXAMPLES OF GUI DATA LOADING
	void hardCodedLoadGUIExample(Game *game);

	// SOME HELPER METHODS FOR SETTING UP THE GAME
	void initColors(GameGraphics *graphics,	map<wstring,wstring> *properties);
	void initViewport(GameGUI *gui, map<wstring,wstring> *properties);
	void makeLuaBot(Game *game, AnimatedSpriteType *botType, const char* luaScriptFile, wstring initialState, float initX, float initY);
	void initAudio(Game *game);

	// AND SOME HELPER METHODS FOR LOADING THE GUI
	void initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initMainMenu(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initWinScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initLoseScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initPauseScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initStoryScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initCreditsScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initLevelChooserScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initControlsScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initPromptScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, wstring yesCommand, wstring noCommand, GameState callingState);

	void initNightRunLevel(Game *game);
	void addBlockTower(float x, float yOffset, int heightInBlocks, Game *game, AnimatedSpriteType* blockType, bool rotate);
	void addBlock(float x, float y, Game *game, AnimatedSpriteType* blockType,bool rotate);
	void addGoalObject(float x, float y, Game *game, AnimatedSpriteType* goalType);
};