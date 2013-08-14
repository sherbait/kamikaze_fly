#include "src\gsm\sprite\Projectile.h"
#include "src\gsm\physics\Physics.h"
#include "src\gsm\state\GameStateManager.h"

Projectile::Projectile()
{
	collisionType = CollisionType::PROJECTILE;
}

void Projectile::init()
{
	GameStateManager* gsm = Game::getInstance()->getGSM();
	Physics* phys = gsm->getPhysics();
	AnimatedSpriteType* type = gsm->getSpriteManager()->getSpriteType(14);
	setSpriteType(type);
	setCurrentState(L"IDLE");
	phys->addCollidableObject(this,0,0,type->getTextureWidth(), type->getTextureHeight(),0, 10, 0, true, 0, true);
	pp.setCollidable(false);
	setAlpha(0);
}

void Projectile::update()
{
	timeToLive -= 1;
	if (timeToLive < 0)
	{
		isDead = true;
	}
}