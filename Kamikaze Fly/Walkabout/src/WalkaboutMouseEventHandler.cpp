#include "stdafx.h"
#include "WalkaboutMouseEventHandler.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gui\Viewport.h"
#include "Player.h"

void WalkaboutMouseEventHandler::handleMousePressEvent(Game *game, int mouseX, int mouseY)
{
	if (game->getGSM()->isGameInProgress())
	{
		Viewport *viewport = game->getGUI()->getViewport();

		// DETERMINE WHERE ON THE MAP WE HAVE CLICKED
		int worldCoordinateX = mouseX + viewport->getViewportX();
		int worldCoordinateY = mouseY + viewport->getViewportY();

		SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
		Player* player = static_cast<Player*>(spriteManager->getPlayer());
		PhysicalProperties* playerPP = spriteManager->getPlayer()->getPhysicalProperties();

		if(!player->getIsDead() && !player->getIsDying() && player->getAmmo() != 0)
		{
			float dx = worldCoordinateX - playerPP->getX();
			float dy = worldCoordinateY - playerPP->getY();
			float distanceToMouse = sqrtf(dx*dx + dy*dy);
			dx /= distanceToMouse;
			dy /= distanceToMouse;

			float bulletOffset = 60;
			float bulletSpeed = 50;

			//Fire projectile
			spriteManager->createProjectile(playerPP->getX() + bulletOffset*dx, playerPP->getY() + bulletOffset*dy,
				bulletSpeed*dx,bulletSpeed*dy);

			player->decrementAmmo();
		}
	}
}

void WalkaboutMouseEventHandler::handleMouseMoveEvent(Game *game, int mouseX, int mouseY)
{
	// DETERMINE WHAT THE PATHFINDING GRID CELL
	// IS THAT THE MOUSE CURRENTLY IS ABOVE
	if (game->getGSM()->isGameInProgress())
	{
		// IF YOU LIKE YOU COULD DO SOME MOUSE OVER DEBUGGING STUFF HERE
	}
}