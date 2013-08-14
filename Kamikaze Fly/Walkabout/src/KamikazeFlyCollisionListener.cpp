#include "src\game\Game.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\KamikazeFlyCollisionListener.h"
#include "src\WalkaboutGame.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"

void KamikazeFlyCollisionListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	//void* bodyUserData1 = contact->GetFixtureA()->GetBody()->GetUserData();
	//void* bodyUserData2 = contact->GetFixtureB()->GetBody()->GetUserData();
	////Check if the body user data is null
	//if ( bodyUserData1 && bodyUserData2  )
	//{
	//	CollidableObject* object1 = static_cast<CollidableObject*>( bodyUserData1 );
	//	CollidableObject* object2 = static_cast<CollidableObject*>( bodyUserData2 );

	//	//If the collision is between the player and a bot, we want to disable the effects of that collision
	//	if ((object1->getCollisionType() == CollisionType::PLAYER && object2->getCollisionType() == CollisionType::BOT)
	//		||(object1->getCollisionType() == CollisionType::BOT && object2->getCollisionType() == CollisionType::PLAYER))
	//	{
	//		contact->SetEnabled(false);
	//	}
	//}
}

void KamikazeFlyCollisionListener::BeginContact(b2Contact* contact)
{

	void* bodyUserData1 = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyUserData2 = contact->GetFixtureB()->GetBody()->GetUserData();
	//Check if the body user data is null
	if ( bodyUserData1 && bodyUserData2  )
	{
		CollidableObject* object1 = static_cast<CollidableObject*>( bodyUserData1 );
		CollidableObject* object2 = static_cast<CollidableObject*>( bodyUserData2 );

		collisionResponseDecider(object1, object2);
	}
	else if(bodyUserData1)
	{
		CollidableObject* object1 = static_cast<CollidableObject*>( bodyUserData1 );
		if(object1->getCollisionType() == CollisionType::PLAYER)
		{
			explodeObject(object1,object1->getPhysicalProperties()->rigidBody);
		}
	}
	else if(bodyUserData2)   
	{
		CollidableObject* object2 = static_cast<CollidableObject*>( bodyUserData2 );
		if(object2->getCollisionType() == CollisionType::PLAYER)
		{
			explodeObject(object2,object2->getPhysicalProperties()->rigidBody);
		}
	}
}

void KamikazeFlyCollisionListener::EndContact(b2Contact* contact)
{
	//void* bodyUserData1 = contact->GetFixtureA()->GetBody()->GetUserData();
	//void* bodyUserData2 = contact->GetFixtureB()->GetBody()->GetUserData();

	////Ensure the player isn't sent reeling at a ridiculous speed after an explosion
	//if (bodyUserData1)
	//{
	//	CollidableObject* object1 = static_cast<CollidableObject*>( bodyUserData1 );
	//	if(object1->getCollisionType()== CollisionType::PLAYER)
	//	{
	//		float vX = object1->getPhysicalProperties()->getVelocityX();
	//		float vY = object1->getPhysicalProperties()->getVelocityY();
	//		if(vX > 2*PLAYER_SPEED)
	//		{
	//			vX = 2*PLAYER_SPEED;
	//			if(vY > 2*PLAYER_SPEED)
	//			{
	//				vY = 2*PLAYER_SPEED;
	//			}
	//			object1->getPhysicalProperties()->setVelocity(vX,vY);
	//		}
	//	}
	//}
	//else if (bodyUserData2)
	//{
	//	CollidableObject* object2 = static_cast<CollidableObject*>( bodyUserData2 );
	//	if(object2->getCollisionType()== CollisionType::PLAYER)
	//	{
	//		float vX = object2->getPhysicalProperties()->getVelocityX();
	//		float vY = object2->getPhysicalProperties()->getVelocityY();
	//		if(vX > 2*PLAYER_SPEED)
	//		{
	//			vX = 2*PLAYER_SPEED;
	//			if(vY > 2*PLAYER_SPEED)
	//			{
	//				vY = 2*PLAYER_SPEED;
	//			}
	//			object2->getPhysicalProperties()->setVelocity(vX,vY);
	//		}
	//	}
	//}
	//else
	//{
	//	CollidableObject* co;
	//	b2Body* body;
	//	if (bodyUserData1)
	//	{
	//		co = static_cast<CollidableObject*>( bodyUserData1 );
	//		body = contact->GetFixtureA()->GetBody();
	//	}
	//	else if (bodyUserData2)
	//	{
	//		co = static_cast<CollidableObject*>( bodyUserData2 );
	//		body = contact->GetFixtureB()->GetBody();
	//	}
	//	else
	//	{
	//		return;
	//	}

	//	if (co->getCollisionType() == CollisionType::PLAYER)
	//	{
	//		explodeObject(co, body);
	//	}
	//	else if (co->getCollisionType() == CollisionType::PROJECTILE)
	//	{
	//		co->setIsDead(true);
	//	}
	//}
}

void  KamikazeFlyCollisionListener::explodeObject(CollidableObject* object, b2Body* body)
{
	int object1x = object->getPhysicalProperties()->getX();
	int object1y = object->getPhysicalProperties()->getY();

	AnimatedSpriteType *explosionSpriteType = determineExplosionSize(object->getPhysicalProperties()->getVelocityX(), object->getPhysicalProperties()->getVelocityY());
	setOffExplosion(game->getGSM()->getPhysics(),object->getPhysicalProperties());
	AnimatedSprite* sprite = static_cast<AnimatedSprite*>( object );
	sprite->setSpriteType(explosionSpriteType);
	sprite->setCurrentState(L"EXPLODE");
	sprite->setIsDying(true);

	sprite->getPhysicalProperties()->disableBox2DBody(true);
	game->getGSM()->getPhysics()->destroyBody(body);
}

AnimatedSpriteType* KamikazeFlyCollisionListener::determineExplosionSize(float vX, float vY)
{
	if( (vX + vY) > 3*PLAYER_SPEED )
	{
		return game->getGSM()->getSpriteManager()->getSpriteType(8);
	}
	else if((vX + vY) > 2*PLAYER_SPEED)
	{
		return game->getGSM()->getSpriteManager()->getSpriteType(9);
	}
	else
	{
		return game->getGSM()->getSpriteManager()->getSpriteType(4);
	}
}

void KamikazeFlyCollisionListener::setOffExplosion(Physics* gamePhysics, PhysicalProperties* objectPP)
{
	float vX = objectPP->getVelocityX();
	float vY = objectPP->getVelocityY();
	Explosion* explosion = new Explosion();
	if( (vX + vY) > 3*PLAYER_SPEED )
	{
		explosion->blastPower = 8000;
		explosion->numOfRays = 400;
		explosion->explosionEpicenter = objectPP->rigidBody->GetWorldCenter();
		explosion->blastRadius = 120;
	}
	else if((vX + vY) > 2*PLAYER_SPEED)
	{
		explosion->blastPower = 4000;
		explosion->numOfRays = 400;
		explosion->explosionEpicenter = objectPP->rigidBody->GetWorldCenter();
		explosion->blastRadius = 90;
	}
	else
	{
		explosion->blastPower = 1000;
		explosion->numOfRays = 400;
		explosion->explosionEpicenter = objectPP->rigidBody->GetWorldCenter();
		explosion->blastRadius = 50;
	}

	gamePhysics->triggerExplosion(explosion);
}

void KamikazeFlyCollisionListener::collisionResponseDecider(CollidableObject* object1, CollidableObject* object2)
{
	switch(object1->getCollisionType())
	{
	case (CollisionType::PLAYER):
		{
			playerCollisionResponder(object1, object2);
			break;
		}
	case (CollisionType::BLOCK):
		{
			blockCollisionResponder(object1, object2);
			break;
		}
	case (CollisionType::BOT):
		{
			botCollisionResponder(object1, object2);
			break;
		}
	case (CollisionType::TILE):
		{
			tileCollisionResponder(object1, object2);
			break;
		}
	case (CollisionType::PROJECTILE):
		{
			projectileCollisionResponder(object1, object2);
			break;
		}
	case (CollisionType::GOAL):
		{
			goalCollisionResponder(object1, object2);
			break;
		}
	}
}

void KamikazeFlyCollisionListener::goalCollisionResponder(CollidableObject* object1, CollidableObject* object2)
{
	switch(object2->getCollisionType())
	{
	case (CollisionType::PLAYER):
		{
			handlePlayerGoalCollision(object2, object1);
			break;
		}
	case (CollisionType::BLOCK):
		{

			break;
		}
	case (CollisionType::BOT):
		{

			break;
		}
	case (CollisionType::TILE):
		{

			break;
		}
	case (CollisionType::PROJECTILE):
		{
			handleProjectileOtherCollision(object2,object1);
			break;
		}
	case (CollisionType::GOAL):
		{

			break;
		}
	}
}

void KamikazeFlyCollisionListener::tileCollisionResponder(CollidableObject* object1, CollidableObject* object2)
{
	switch(object2->getCollisionType())
	{
	case (CollisionType::PLAYER):
		{
			handlePlayerTileCollision(object2, object1);
			break;
		}
	case (CollisionType::BLOCK):
		{

			break;
		}
	case (CollisionType::BOT):
		{

			break;
		}
	case (CollisionType::TILE):
		{

			break;
		}
	case (CollisionType::PROJECTILE):
		{
			handleProjectileOtherCollision(object2,object1);
			break;
		}
	case (CollisionType::GOAL):
		{

			break;
		}
	}
}

void KamikazeFlyCollisionListener::playerCollisionResponder(CollidableObject* object1, CollidableObject* object2)
{
	switch(object2->getCollisionType())
	{
	case (CollisionType::PLAYER):
		{

			break;
		}
	case (CollisionType::BLOCK):
		{
			handlePlayerBlockCollision(object1, object2);
			break;
		}
	case (CollisionType::BOT):
		{
			handlePlayerBotCollision(object1, object2);
			break;
		}
	case (CollisionType::TILE):
		{
			handlePlayerTileCollision(object1, object2);
			break;
		}
	case (CollisionType::PROJECTILE):
		{
			handlePlayerProjectileCollision(object1, object2);
			break;
		}
	case (CollisionType::GOAL):
		{
			handlePlayerGoalCollision(object1, object2);
			break;
		}
	}
}

void KamikazeFlyCollisionListener::botCollisionResponder(CollidableObject* object1, CollidableObject* object2)
{
	switch(object2->getCollisionType())
	{
	case (CollisionType::PLAYER):
		{
			handlePlayerBotCollision(object2, object1);
			break;
		}
	case (CollisionType::BLOCK):
		{

			break;
		}
	case (CollisionType::BOT):
		{

			break;
		}
	case (CollisionType::TILE):
		{

			break;
		}
	case (CollisionType::PROJECTILE):
		{
			handleProjectileBotCollision(object2, object1);
			break;
		}
	case (CollisionType::GOAL):
		{

			break;
		}
	}
}

void KamikazeFlyCollisionListener::blockCollisionResponder(CollidableObject* object1, CollidableObject* object2)
{
	switch(object2->getCollisionType())
	{
	case (CollisionType::PLAYER):
		{
			handlePlayerBlockCollision(object2, object1);
			break;
		}
	case (CollisionType::BLOCK):
		{

			break;
		}
	case (CollisionType::BOT):
		{

			break;
		}
	case (CollisionType::TILE):
		{

			break;
		}
	case (CollisionType::PROJECTILE):
		{
			handleProjectileOtherCollision(object2,object1);
			break;
		}
	case (CollisionType::GOAL):
		{

			break;
		}
	}
}

void KamikazeFlyCollisionListener::projectileCollisionResponder(CollidableObject* object1, CollidableObject* object2)
{
	switch(object2->getCollisionType())
	{
	case (CollisionType::PLAYER):
		{
			handlePlayerProjectileCollision(object2, object1);
			break;
		}
	case (CollisionType::BLOCK):
		{

			break;
		}
	case (CollisionType::BOT):
		{
			handleProjectileBotCollision(object1, object2);
			break;
		}
	case (CollisionType::TILE):
		{
			handleProjectileOtherCollision(object1,object2);
			break;
		}
	case (CollisionType::PROJECTILE):
		{
			handleProjectileOtherCollision(object1,object2);
			break;
		}
	case (CollisionType::GOAL):
		{
			handleProjectileOtherCollision(object1,object2);
			break;
		}
	}
}

void KamikazeFlyCollisionListener::handlePlayerBotCollision(CollidableObject* object1, CollidableObject* object2)
{
	if(object1->getIsFalling() && !object1->getIsDying() && !object2->getIsDying()
		&& !object1->getIsDead() && !object2->getIsDead())
	{
		explodeObject(object1, object1->getPhysicalProperties()->rigidBody);
		explodeObject(object2, object2->getPhysicalProperties()->rigidBody);
	}
	else if(!object1->getIsDying() && !object2->getIsDying()
		&& !object1->getIsDead() && !object2->getIsDead())
	{
		object1->setIsFalling(true);
		AnimatedSprite* sprite = static_cast<AnimatedSprite*>( object1);
		sprite->setCurrentState(L"FALLING");
		explodeObject(object2, object2->getPhysicalProperties()->rigidBody);
	}
}
void KamikazeFlyCollisionListener::handlePlayerGoalCollision(CollidableObject* object1, CollidableObject* object2)
{
	if(!object1->getIsDying() && !object2->getIsDying()
		&& !object1->getIsDead() && !object2->getIsDead())
	{
		explodeObject(object1, object1->getPhysicalProperties()->rigidBody);
		explodeObject(object2, object2->getPhysicalProperties()->rigidBody);
	}
}
void KamikazeFlyCollisionListener::handlePlayerTileCollision(CollidableObject* object1, CollidableObject* object2)
{
	if(!object1->getIsDying() && !object1->getIsDead())
	{
		explodeObject(object1, object1->getPhysicalProperties()->rigidBody);
	}
}
void KamikazeFlyCollisionListener::handlePlayerBlockCollision(CollidableObject* object1, CollidableObject* object2)
{
	if(!object1->getIsDying() && !object2->getIsDying()
		&& !object1->getIsDead() && !object2->getIsDead())
	{
		explodeObject(object1, object1->getPhysicalProperties()->rigidBody);
		explodeObject(object2, object2->getPhysicalProperties()->rigidBody);
	}
}
void KamikazeFlyCollisionListener:: handlePlayerProjectileCollision(CollidableObject* object1, CollidableObject* object2)
{
	if(!object1->getIsDying() && !object2->getIsDying()
		&& !object1->getIsDead() && !object2->getIsDead())
	{
		object2->setIsDead(true);
		explodeObject(object1, object1->getPhysicalProperties()->rigidBody);
	}
}

void KamikazeFlyCollisionListener:: handleProjectileBotCollision(CollidableObject* object1, CollidableObject* object2)
{
	if(!object1->getIsDying() && !object2->getIsDying()
		&& !object1->getIsDead() && !object2->getIsDead())
	{
		object1->setIsDead(true);
		explodeObject(object2, object2->getPhysicalProperties()->rigidBody);
	}
}

void KamikazeFlyCollisionListener::handleProjectileOtherCollision(CollidableObject* object1, CollidableObject* object2)
{
	object1->setIsDead(true);
}
