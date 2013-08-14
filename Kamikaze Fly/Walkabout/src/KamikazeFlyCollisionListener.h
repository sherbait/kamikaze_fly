#pragma once

#include "Box2D\Box2D.h"
#include "src\gsm\physics\Physics.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"

class KamikazeFlyCollisionListener : public b2ContactListener
{
private:
	Game* game;
	AnimatedSpriteType* determineExplosionSize(float velocityX, float velocityY);
	void setOffExplosion(Physics* gamePhysics, PhysicalProperties* objectPP);
	void explodeObject(CollidableObject* object, b2Body* body);

	void collisionResponseDecider(CollidableObject* object1, CollidableObject* object2);
	void projectileCollisionResponder(CollidableObject* object1, CollidableObject* object2);
	void playerCollisionResponder(CollidableObject* object1, CollidableObject* object2);
	void goalCollisionResponder(CollidableObject* object1, CollidableObject* object2);
	void botCollisionResponder(CollidableObject* object1, CollidableObject* object2);
	void blockCollisionResponder(CollidableObject* object1, CollidableObject* object2);
	void tileCollisionResponder(CollidableObject* object1, CollidableObject* object2);

	void handlePlayerBotCollision(CollidableObject* object1, CollidableObject* object2);
	void handlePlayerGoalCollision(CollidableObject* object1, CollidableObject* object2);
	void handlePlayerTileCollision(CollidableObject* object1, CollidableObject* object2);
	void handlePlayerBlockCollision(CollidableObject* object1, CollidableObject* object2);
	void handlePlayerProjectileCollision(CollidableObject* object1, CollidableObject* object2);
	
	void handleProjectileBotCollision(CollidableObject* object1, CollidableObject* object2);
	void handleProjectileOtherCollision(CollidableObject* object1, CollidableObject* object2);

public:
	KamikazeFlyCollisionListener(Game* theGame)
	{
		game = theGame;
	}

	virtual void BeginContact(b2Contact* contact) override;

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

	virtual void EndContact(b2Contact* contact) override;
};
