/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutTextGenerator.cpp

	See WalkaboutTextGenerator.h for a class description.
*/

#include "stdafx.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutTextGenerator.h"
#include "src\game\Game.h"
#include "src\graphics\GameGraphics.h"
#include "src\text\GameText.h"
#include "src\timer\GameTimer.h"
#include "src\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

static const wstring	W_TARGET_FPS_TEXT = L"TARGET FPS: ";
static const int		W_TARGET_FPS_X	= 350;
static const int		W_TARGET_FPS_Y	= 50;

/*
	appendTargetFPS - this method gets and appends the target frame rate to
	the text we are generating.
*/
void WalkaboutTextGenerator::appendTargetFPS(Game *game)
{
	// WE'LL BUILD OUR WIDE STRING USING A WIDE STRING STREAM
	/*wstringstream wss;
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();*/

	// BUILD THE WIDE STRING
	/*textToGenerate.append(W_TARGET_FPS_TEXT);
	wss << timer->getTargetFPS();
	textToGenerate.append(wss.str());*/
}

void WalkaboutTextGenerator::showHint(Game *game)
{
	wstringstream wss;
	if ((game->getGSM()->getCurrentGameState() == GS_GAME_IN_PROGRESS) &&
		(game->getCurrentLevelFileName().compare(W_LEVEL_1_NAME) == 0))	// Hints are only shown in Level 1 (aka Tutorial Level)
	{
		// Get player's coordinates. The coordinates will determine when to show specific hints.
		// We actually only need the player's x-coordinate here.
		float playerX = game->getGSM()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getX();
		float playerY = game->getGSM()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getY();

		if (!game->getGSM()->getSpriteManager()->getPlayer()->getIsDead())
		{
			if (playerX < 256)
			{
				hint.append(L"Greetings, Soldier.");
			}
			else if (playerX > 320 && playerX < 1280)
			{
				hint.append(L"Press 'S' to FLY DOWN.");
			}
			else if (playerX > 1312 && playerX < 2080)
			{
				hint.append(L"Press 'W' to FLY UP.");
			}
			else if (playerX > 2144 && playerX < 3648)
			{
				hint.append(L"Press 'D' to SPEED UP and 'A' to SLOW DOWN.");
			}
			else if (playerX > 3712 && playerX < 4480)
			{
				hint.append(L"YOUR MISSION: Crash onto and destroy Eastasian critter eggs.");
			}
			else if (playerX > 4544 && playerX < 5952)
			{
				hint.append(L"Avoid the proletariat denizens of Eastasia at all costs...");
			}
			else if (playerX > 6016 && playerX < 6976)
			{
				hint.append(L"...or press your LEFT-MOUSE button to fire GNAT missiles at them.");
			}
			else if (playerX > 7008 && playerX < 8192)
			{
				hint.append(L"Eliminate all Eastasian eggs, and your mission is ACCOMPLISHED.");
			}
		}
	}
}

void WalkaboutTextGenerator::showOrbCount(Game *game)
{
	wstringstream wss;
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	if (game->getGSM()->getCurrentGameState() == GS_GAME_IN_PROGRESS)
	{
		int orbCount = spriteManager->getNumberOfGoalObjects();
		wss << L"Remaining Orbs: " << orbCount;
		orbs.append(wss.str());
	}
}

/*
	initText - Provides an example of how to render text to our screen every frame,
	allowing for the text to continually change. This method adds a text object
	to the text manager (GameText), which allows it to be updated each frame. This
	method should be called only once for each object.
*/
void WalkaboutTextGenerator::initText(Game *game)
{
	// FIRST UPDATE THE TEXT THIS GENERATOR IS USING
	//appendTargetFPS(game);

	// AND THEN HAND IT TO THE TEXT MANAGER, SPECIFYING WHERE IT SHOULD BE RENDERED
	GameText *text = game->getText();
	//text->addText(&textToGenerate, 350, 50, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addText(&orbs, game->getGraphics()->getScreenWidth() - 250, 20, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addText(&hint, 200, 150, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
}

/*
	updateText - updates the text. Note that the text must already have been added
	to the text manager via the initText method. Also not that there should be a
	separate TextGenerator for each line of text we wish to render.
*/
void WalkaboutTextGenerator::updateText(Game *game)
{
	// THE TEXT MANAGER (GameText) HAS A POINTER TO OUR TEXT, SO WE CAN
	// SIMPLY CLEAR IT OUT OF OLD CHARACTER DATA AND FILL IT IN WITH
	// WHAT WE WANT.
	//textToGenerate.clear();
	//appendTargetFPS(game);

	hint.clear();
	showHint(game);

	orbs.clear();
	showOrbCount(game);
}