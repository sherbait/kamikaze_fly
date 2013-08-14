/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

Physics.cpp

See Physics.h for a class description.
*/

#include "stdafx.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"
#include "src\gsm\physics\Collision.h"
#include "src\gsm\physics\CollidableObject.h"
#include "src\game\Game.h"
#include "src\gsm\physics\Physics.h"
#include "src\gsm\world\SparseLayer.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\gsm\world\TiledLayer.h"
#include "src\gsm\world\Tile.h"
#include "src\gsm\world\World.h"
#include "src\gsm\world\WorldLayer.h"
#include <vector>
#include <list>
#include "ExplosionCallback.h"
#include "Box2D\Box2D.h"

static Physics* instance;

/*
The constructor initializes the data structures and loads
the necessary ones with recyclable objects for collision tests.
*/
Physics::Physics()
{
	b2Vec2 gravity(0.0f, DEFAULT_GRAVITY);
	box2DWorld = new b2World(gravity);
	explosionThisFrame = false;
}

/*
The destructor deletes all the recyclable objects.
*/
Physics::~Physics()
{

}

Physics* Physics::getInstance()
{
	if (!instance)
	{
		instance = new Physics();
	}
	return instance;
}

void Physics::setOffExplosion(b2Vec2 position, int power, int numRays, int radius)
{
	Explosion* explosion = new Explosion();
	explosion->blastPower = power;
	explosion->numOfRays = numRays;
	explosion->explosionEpicenter = position;
	explosion->blastRadius = radius;

	triggerExplosion(explosion);
}

/*
Informs the physics system of additional dynamic, collidable object
to keep track of. Note that this method should not be used mid-collision, 
as a result of a collition event, since it does not update all the
swept shape positions. Also note that it does not test to see if the 
added object overlaps an object already in the game.
*/
//////////////////////////KEEP/////////////////////////////////////////////
void Physics::addCollidableObject(CollidableObject *collidableObjectToAdd)
{
	PhysicalProperties *pp = collidableObjectToAdd->getPhysicalProperties();
	float height = pixelsToMeters(collidableObjectToAdd->getBoundingVolume()->getHeight()) / 2;
	float width = pixelsToMeters(collidableObjectToAdd->getBoundingVolume()->getWidth()) / 2;
	float x = pixelsToMeters(pp->getX());
	float y = pixelsToMeters(-pp->getY());

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	b2Body* body = box2DWorld->CreateBody(&bodyDef);

	testSubjectBody = body;

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(width, height);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);
}

void Physics::addDynamicRectangle(CollidableObject* co, 
								  float centerX, 
								  float centerY, 
								  int width, 
								  int height, 
								  float angle, 
								  float density, 
								  float friction, 
								  float gravityScale)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pixelsToMeters(centerX),-pixelsToMeters(centerY));
	bodyDef.gravityScale = gravityScale;
	bodyDef.angle = angle;
	co->getPhysicalProperties()->rigidBody = box2DWorld->CreateBody(&bodyDef);
	b2Body *rigidBody = co->getPhysicalProperties()->rigidBody;
	rigidBody->SetUserData(co);
	b2FixtureDef fixtureDef;
	setCollisionFilters(co, &fixtureDef);
	b2PolygonShape box;
	box.SetAsBox(pixelsToMeters(width/2), pixelsToMeters(height/2));
	fixtureDef.shape = &box;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = .03f;
	co->getPhysicalProperties()->rigidBody->CreateFixture(&fixtureDef);
	co->getPhysicalProperties()->setWidth(width);
	co->getPhysicalProperties()->setHeight(height);
}

void Physics::addCollidableObject(CollidableObject* co, float x, float y, int width, int height, float angle,
								  float density, float friction, bool fixedRotation, float gravityScale, bool isCircular)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	//The bodyDef position is actually the position of the center of mass
	bodyDef.position.Set(pixelsToMeters(x + width/2),-pixelsToMeters(y + height/2));
	bodyDef.gravityScale = gravityScale;
	bodyDef.angle = angle;
	co->getPhysicalProperties()->rigidBody = box2DWorld->CreateBody(&bodyDef);
	b2Body *rigidBody = co->getPhysicalProperties()->rigidBody;
	rigidBody->SetUserData(co);
	
	b2FixtureDef fixtureDef;
	setCollisionFilters(co, &fixtureDef);

	b2PolygonShape box;
	b2CircleShape circle;
	if(!isCircular)
	{
		box.SetAsBox(pixelsToMeters(width/2), pixelsToMeters(height/2));
		fixtureDef.shape = &box;
	}
	else
	{
		//The circle shape should be centered over the body
		circle.m_p.Set(-pixelsToMeters(width/2),-pixelsToMeters(height/2));
		//Width and height should be equal, so it doesn't matter which we use here for the radius
		circle.m_radius = pixelsToMeters(width/2);
		fixtureDef.shape = &circle;
	}
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = .03f;
	co->getPhysicalProperties()->rigidBody->CreateFixture(&fixtureDef);
	co->getPhysicalProperties()->rigidBody->SetFixedRotation(fixedRotation);
	co->getPhysicalProperties()->setWidth(width);
	co->getPhysicalProperties()->setHeight(height);
}

void Physics::addStaticRectangle(int x, int y, float width, float height)
{
	b2BodyDef bodyDef;

	//The bodyDef position is actually the position of the center of mass
	bodyDef.position.Set(pixelsToMeters(x + width/2),-pixelsToMeters(y + height/2));
	b2Body* body = box2DWorld->CreateBody(&bodyDef);
	
	b2FixtureDef fixtureDef;

	b2PolygonShape box;
	box.SetAsBox(pixelsToMeters(width/2), pixelsToMeters(height/2));
	fixtureDef.shape = &box;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);
}

/*
Removes a dynamic, collidable object from the physics system. Note that
this method should not be used mid-collision, as a result of a collision 
event, since it does not update all the swept shape positions. Also note 
that it does not test to see if the added object overlaps an object already 
in the game.
*/
//////////////////////////KEEP/////////////////////////////////////////////
void Physics::removeCollidableObject(CollidableObject *collidableObjectToRemove)
{

}

void Physics::addCollidableTiles(TiledLayer *collidableTiles)
{
	if(!collidableTiles->hasCollidableTiles())
	{
		//Why are you wasting time calling this method on a layer that doesn't even have collidable tiles?
		//I'd throw an exception, but that's too much work.
		return;
	}

	int rows = collidableTiles->getRows();
	int columns = collidableTiles->getColumns();
	int tileWidth = collidableTiles->getTileWidth();
	int tileHeight = collidableTiles->getTileHeight();

	for(int i = 0; i < rows; i++)
	{
		for(int k = 0; k < columns; k++)
		{
			Tile *tile = collidableTiles->getTile(i,k);

			if(tile->collidable)
			{
				//The bodyDef position is actually the position of the center of mass, so we add half the height and width to the original x coordinate
				b2BodyDef tileBodyDef;
				float tileXScaled = pixelsToMeters(tileWidth * k + tileWidth/2);
				float tileYScaled = -pixelsToMeters(tileHeight * i + tileHeight/2);
				float tileWidthScaled = pixelsToMeters(tileWidth)/2.0;
				float tileHeightScaled = pixelsToMeters(tileHeight)/2.0;
				tileBodyDef.position.Set(tileXScaled, tileYScaled);

				// Call the body factory which allocates memory for the ground body
				// from a pool and creates the ground box shape (also from a pool).
				// The body is also added to the world.
				b2Body* tileBody = box2DWorld->CreateBody(&tileBodyDef);

				//!!! THIS MAY CAUSE A MEMORY LEAK IF WE DESTORY THE TILE BODY WITHOUT ALSO DESTROYING THE USER DATA
				tileBody->SetUserData(new CollidableObject(CollisionType::TILE));

				//We need to keep a reference to the body so that we can destroy it 
				//later when we no longer need it
				//--------WE NEED TO STORE THE TILE BODIES IN A DATA STRUCTURE HERE----//

				// Define the ground box shape.
				b2PolygonShape groundBox;

				// The extents are the half-widths of the box.
				groundBox.SetAsBox(tileWidthScaled, tileHeightScaled);

				// Add the ground fixture to the ground body.
				tileBody->CreateFixture(&groundBox, 0.0f);
			}
		}
	}
}

void Physics::update(Game *game)
{
	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// This is our little game loop.
	for (int32 i = 0; i < 60; ++i)
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		box2DWorld->Step(timeStep, velocityIterations, positionIterations);
	}

	list<b2Body*>::iterator bodiesIterator;
	bodiesIterator = bodiesToDestroy.begin();
	while (bodiesIterator != bodiesToDestroy.end())
	{
		b2Body* body = *bodiesIterator;
		bodiesIterator++;
		box2DWorld->DestroyBody(body);
 	}
	bodiesToDestroy.clear();

	if(explosionThisFrame)
	{
		list<Explosion*>::iterator explosionsIterator;
		explosionsIterator = explosions.begin();
		while (explosionsIterator != explosions.end())
		{
			Explosion* explosion = *explosionsIterator;
			explosionsIterator++;

			int numRays = explosion->numOfRays;
			b2Vec2 center = explosion->explosionEpicenter;
			float blastRadius = explosion->blastRadius; 
			for (int i = 0; i < numRays; i++) 
			{
				float angle = (i / (float)numRays) * 360 * DEGTORAD;
				b2Vec2 rayDir( sinf(angle), cosf(angle) );
				b2Vec2 rayEnd = center + pixelsToMeters(blastRadius) * rayDir;

				//check what this ray hits
				ExplosionCallback callback;//basic callback to record body and hit point
				box2DWorld->RayCast(&callback, center, rayEnd);
				if ( callback.getClosestFixture() ) 
					applyBlastImpulse(callback.getClosestFixture()->GetBody(), center, callback.getImplusePoint(), (explosion->blastPower / (float)numRays));
			}
		}
		explosions.clear();
		explosionThisFrame = false;
	}
}

void Physics::setCollisionListener(b2ContactListener *initCollisionListener)
{
	box2DWorld->SetContactListener(initCollisionListener);
}

void Physics::applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower)
{
	b2Vec2 blastDir = applyPoint - blastCenter;
      float distance = blastDir.Normalize();
      //ignore bodies exactly at the blast point - blast direction is undefined
      if ( distance == 0 )
          return;
      float invDistance = 1 / distance;
      float impulseMag = blastPower * invDistance * invDistance;
      body->ApplyLinearImpulse( impulseMag * blastDir, applyPoint );
}

void Physics::setCollisionFilters(CollidableObject* co, b2FixtureDef* fixtureDef)
{
	CollisionType type = co->getCollisionType();
	fixtureDef->filter.categoryBits = type;
	int mask;
	switch (type)
	{
	case (CollisionType::BLOCK):
		{
			mask = 0xFFFF;
			break;
		}
	case (CollisionType::BOT):
		{
			mask = CollisionType::PLAYER | CollisionType::PROJECTILE | CollisionType::TILE;
			break;
		}
	case (CollisionType::GOAL):
		{
			mask = 0xFFFF;
			break;
		}
	case (CollisionType::PLAYER):
		{
			mask = CollisionType::GOAL | CollisionType::BLOCK
				| CollisionType::TILE | CollisionType::BOT;
			break;
		}
	case (CollisionType::PROJECTILE):
		{
			mask = CollisionType::GOAL | CollisionType::BLOCK
				| CollisionType::BOT;
			break;
		}
	case (CollisionType::TILE):
		{
			0xFFFF;
			break;
		}
	default:
		mask = 0;
		break;
	}
	fixtureDef->filter.maskBits = mask;
}