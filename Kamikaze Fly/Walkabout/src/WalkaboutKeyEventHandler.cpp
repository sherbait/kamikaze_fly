/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

WalkaboutKeyEventHandler.cpp

See WalkaboutKeyEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutKeyEventHandler.h"
#include "src\game\Game.h"
#include "src\game\WStringTable.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\physics\Physics.h"
#include "src\gsm\physics\PhysicalProperties.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\state\GameState.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gui\Cursor.h"
#include "src\gui\GameGUI.h"
#include "src\input\GameInput.h"
#include "src\timer\GameTimer.h"
#include "src\PlatformPlugins\WindowsPlugin\WindowsTimer.h"
#include "Player.h"

/*
handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
key states since the last frame, which can allow us to respond to key presses, including when keys
are held down for multiple frames.
*/
void WalkaboutKeyEventHandler::handleKeyEvents(Game *game)
{
	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	Player* player = static_cast<Player*>(gsm->getSpriteManager()->getPlayer());
	PhysicalProperties *pp = player->getPhysicalProperties();

	// USED TO TEST IF THE PLAYER PAUSED IN THIS FRAME. IF HE/SHE DID,
	// DISPLAY THE PAUSE GUI; OTHERWISE, "CLEAR" THE GUI.
	bool pausedThisFrame = false;

	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		if (input->isKeyDown(VK_CONTROL))
		{
			if (input->isKeyDown(VK_NUMPAD1))
			{
				game->setCurrentLevelFileName(W_LEVEL_1_NAME);
				game->startGame();
			}
			else if (input->isKeyDown(VK_NUMPAD2))
			{
				game->setCurrentLevelFileName(W_LEVEL_2_NAME);
				game->startGame();
			}
			else if (input->isKeyDown(VK_NUMPAD3))
			{
				game->setCurrentLevelFileName(W_LEVEL_3_NAME);
				game->startGame();
			}
		}


		if(!player->getIsDying() && !player->getIsDead())
		{

			// WS KEY PRESSES WILL CONTROL THE PLAYER
			// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
			// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
			if(!player->getIsFalling())
			{
				if (input->isKeyDown(W_KEY))
				{
					if(pp->getVelocityY() <= -2*PLAYER_SPEED)
					{
						pp->setVelocity(pp->getVelocityX(), -2*PLAYER_SPEED);
					}
					else
					{
						int worldHeight = gsm->getWorld()->getWorldHeight();
						//No upward acceleration can be applied if we are near the top boundary of the level
						if(pp->getY() >= 5)
						{
							pp->rigidBody->ApplyForce(b2Vec2(0,550), pp->rigidBody->GetWorldCenter());
						}
					}
				}

				if (input->isKeyDown(S_KEY))
				{
					player->setCurrentState(L"ACCELERATING_DOWN");
					if(pp->getVelocityY() >= 2*PLAYER_SPEED)
					{
						pp->setVelocity(pp->getVelocityX(), 2*PLAYER_SPEED);
					}
					else
					{
						pp->rigidBody->ApplyForce(b2Vec2(0,-550), pp->rigidBody->GetWorldCenter());
					}
				}
				else if(player->getCurrentState() == L"ACCELERATING_DOWN")
				{
					player->setCurrentState(L"IDLE");
				}

				if (input->isKeyDown(D_KEY))
				{
					if(pp->getVelocityX() >= 2*PLAYER_SPEED)
					{
						pp->setVelocity(2*PLAYER_SPEED, pp->getVelocityY());
					}
					else
					{
						pp->rigidBody->ApplyForce(b2Vec2(550,0), pp->rigidBody->GetWorldCenter());
					}
				}

				if (input->isKeyDown(A_KEY))
				{
					if(pp->getVelocityX() <= (PLAYER_SPEED/2) )
					{
						pp->setVelocity(PLAYER_SPEED/2, pp->getVelocityY());
					}
					else
					{
						pp->rigidBody->ApplyForce(b2Vec2(-550,0), pp->rigidBody->GetWorldCenter());
					}

				}
			}
			else
			{
				pp->rigidBody->ApplyForce(b2Vec2(0,-550), pp->rigidBody->GetWorldCenter());
			}

			//Keep the player from flying out of the top of the level
			float playerY = pp->getY();
			float initVy = pp->getVelocityY();
			if(playerY < 5 && initVy < 0)
			{
				float initVx = pp->getVelocityX();
				pp->setVelocity(initVx, 0);
			}

			//Rotation is proportional to the vertical speed...
			float rotationCoeffecient = abs(pp->getVelocityY() / (2*PLAYER_SPEED));
			//...and inversely proportional to the horizontal speed...
			float rotationDampeningDueToFowardVelocity = (PLAYER_SPEED/2) / pp->getVelocityX();
			//...this gives a better illusion of aerodynamics, even though I have no idea what the correct
			//way to calculate these proportions is :3
			float rotation = 90 * rotationCoeffecient * rotationDampeningDueToFowardVelocity;

			if(pp->getVelocityY() >= 0 && rotation <= 45)
			{
				pp->setRotation(rotation);
			}
			else if (rotation <= 45)
			{
				pp->setRotation(-rotation);
			}
		}

		bool viewportMoved = false;
		float viewportVx = 0.0f;

		Viewport *viewport = game->getGUI()->getViewport();

		float playerX = pp->getX();
		float viewportX = viewport->getViewportX();

		bool moveViewport = playerX - 100 > viewportX;

		if(moveViewport && !player->getIsDead())
		{
			viewport->moveViewport(pp->getVelocityX(), 0, game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		}
		else if(player->getIsDead())
		{
			float remainingDistance= floor(viewportX) - (game->getWaypointX() + 80);

			if(remainingDistance != 0)
			{
				if(remainingDistance > 0)
				{
					if(remainingDistance < 3*PLAYER_SPEED)
					{
						viewport->moveViewport((int)-remainingDistance, 0, game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
					}
					else
					{
						viewport->moveViewport((int)-3*PLAYER_SPEED, 0, game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
					}
				}
				else if(remainingDistance < 0)
				{
					if(abs(remainingDistance) < 3*PLAYER_SPEED)
					{
						viewport->moveViewport((int)abs(remainingDistance), 0, game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
					}
					else
					{
						viewport->moveViewport((int)3*PLAYER_SPEED, 0, game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
					}
				}
			}
			else
			{
				player->setIsDead(false);
				player->getPhysicalProperties()->disableBox2DBody(false);
				AnimatedSpriteType *playerSpriteType = gsm->getSpriteManager()->getSpriteType(2);
				player->setSpriteType(playerSpriteType);
				player->setCurrentState(IDLE);
				player->setIsFalling(false);
				player->setAmmo(AMMO);
				gsm->getPhysics()->addCollidableObject(player, game->getWaypointX(), PLAYER_INIT_Y, playerSpriteType->getTextureWidth(), 
					playerSpriteType->getTextureHeight(),0, 20.0f, 0.0f, true, 0,false);
				player->getPhysicalProperties()->setVelocity(PLAYER_SPEED, 0);
				player->setCurrentState(L"IDLE");
			}
		}

		// DOES THE PLAYER WANT TO PAUSE THE GAME?
		if (input->isKeyDownForFirstTime(ESC_KEY))
		{
			gsm->goToPause();
			pausedThisFrame = true;
		}
	}

	// IS THE GAME PAUSED? WAS IT PAUSED IN THE SAME FRAME OR THE PREVIOUS FRAME?
	if (gsm->isGamePaused() && !pausedThisFrame)
	{
		// PLAYER WANTS TO UNPAUSE GAME
		if (input->isKeyDownForFirstTime(ESC_KEY))
		{
			gsm->goToGame();
		}
	}

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		unsigned int id = cursor->getActiveCursorID();
		id++;
		if (id == cursor->getNumCursorIDs())
			id = 0;		
		cursor->setActiveCursorID(id);
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();
	int fps = timer->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_HOME) && (fps < MAX_FPS))
		timer->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_END) && (fps > MIN_FPS))
		timer->setTargetFPS(fps - FPS_INC);
}