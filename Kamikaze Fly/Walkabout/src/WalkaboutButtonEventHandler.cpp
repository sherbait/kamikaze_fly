/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutButtonEventHandler.cpp

	See WalkaboutButtonEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutButtonEventHandler.h"
#include "src\game\Game.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\audio\AudioManager.h"

void WalkaboutButtonEventHandler::handleButtonEvents(	Game *game, 
													wstring command)
{
	GameStateManager *gsm = game->getGSM();

	// Do we have to play a button-click sound?
	game->getAudioManager()->respondToButtonPress(game);

	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(W_EXIT_COMMAND) == 0)
	{
		game->shutdown();
	}
	// THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
	// SCREEN, SO LET'S GO TO THE MAIN MENU
	else if (command.compare(W_GO_TO_MM_COMMAND) == 0)
	{
		gsm->goToMainMenu();
	}
	// THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
	// SO LET'S START THE GAME FROM THE FIRST LEVEL
	/*else if (command.compare(W_START_COMMAND) == 0)
	{
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		game->startGame();
	}*/
	// The user pressed the NEW GAME button on the main menu
	// so let's display the level chooser.
	else if (command.compare(W_LEVEL_COMMAND) == 0)
	{
		gsm->goToLevelChooser();
	}
	else if (command.compare(W_LEVEL_1_COMMAND) == 0)
	{
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		game->quitGame();
		game->startGame();
	}
	else if (command.compare(W_LEVEL_2_COMMAND) == 0)
	{
		game->setCurrentLevelFileName(W_LEVEL_2_NAME);
		game->quitGame();
		game->startGame();
	}
	else if (command.compare(W_LEVEL_3_COMMAND) == 0)
	{
		game->setCurrentLevelFileName(W_LEVEL_3_NAME);
		game->quitGame();
		game->startGame();
	}
	else if (command.compare(W_PAUSE_COMMAND) == 0)
	{
		gsm->goToPause();
	}
	// THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
	// SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
	else if (command.compare(W_QUIT_COMMAND) == 0)
	{
		game->quitGame();
	}
	// USER PRESSED THE STORY BUTTON ON THE MAIN MENU OR
	// BACKED FROM THE SECOND STORY SCREEN
	else if (command.compare(W_STORY_1_COMMAND) == 0)
	{
		gsm->goToStory();
	}
	// USER PRESSED THE RIGHT ARROW FROM STORY 1
	else if (command.compare(W_STORY_2_COMMAND) == 0)
	{
		gsm->goToStory();
	}
	else if (command.compare(W_CREDITS_COMMAND) == 0)
	{
		gsm->goToCredits();
	}
	else if (command.compare(W_CONTROLS_COMMAND) == 0)
	{
		gsm->goToControls();
	}
	else if (command.compare(W_PROMPT_COMMAND) == 0)
	{
		gsm->goToPrompt();
	}
}