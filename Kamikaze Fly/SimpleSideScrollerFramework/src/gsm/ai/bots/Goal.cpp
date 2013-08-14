#include "src\game\Game.h"
#include "src\gsm\state\GameStateManager.h"
#include "Box2D\Box2D.h"
#include "src\gsm\ai\bots\Goal.h"

void Goal::think(Game* game)
{
	float velocity = sqrtf(pp.getVelocityX()*pp.getVelocityX() + pp.getVelocityY()*pp.getVelocityY());
	if (fabsf(previousVelocity-velocity) > maxVelocity)
	{
		setSpriteType(Game::getInstance()->getGSM()->getSpriteManager()->getSpriteType(4));
		setCurrentState(L"EXPLODE");
		setIsDying(true);
		if (!pp.isDisabled())
		{
			pp.disableBox2DBody(true);
			Physics::getInstance()->destroyBody(pp.getBody());
		}
	}
	previousVelocity = velocity;
}
