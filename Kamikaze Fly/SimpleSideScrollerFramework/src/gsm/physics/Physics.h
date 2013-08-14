/*	
Author: Richard McKenna

Physics.h

This class will be used to manage all game collisions
and physics. This includes collision detection, and resolution.
Each frame we will update the collided objects velocities and
positions accordingly.
*/
#pragma once
#include "stdafx.h"
#include "src\game\Game.h"
#include "src\gsm\physics\Collision.h"
#include "src\gsm\physics\CollidableObject.h"
#include "src\gsm\physics\CollisionListener.h"
#include "src\gsm\world\World.h"
#include "src\gsm\world\TiledLayer.h"
#include "src\gsm\world\Tile.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "Explosion.h"

#include "Box2D\Box2D.h"

#define DEGTORAD 0.0174532925199432957f

const float DEFAULT_GRAVITY = -0.3f;
const float SCALING_FACTOR = 100.0f;

class Physics
{

public:

	//Our Box 2D representation of the game world.
	b2World *box2DWorld;

	list<Explosion*> explosions;
	bool explosionThisFrame;

	list<b2Body*> bodiesToDestroy;

	CollidableObject *testSubject;
	b2Body *testSubjectBody;

	// THIS IS APPLIED TO EVERY SPRITE EVERY FRAME
	float gravity;

	// USED FOR TESTING PHYSICS BY TURNING IT ON AND OFF IN VARIOUS WAYS
	//bool activated;
	//bool activatedForSingleUpdate;

	// INLINED GET/SET METHODS
	float				getGravity()					{ return gravity;					}
	b2World*			getWorld()					{ return box2DWorld;					}
	//bool				isActivated()					{ return activated;					}
	//bool				isActivatedForSingleUpdate()	{ return activatedForSingleUpdate;	}
	//void				activateForSingleUpdate()										{ activatedForSingleUpdate = true;				}
	void				setGravity(float initGravity)									{ gravity = initGravity;						}
	void				setCollisionListener(b2ContactListener *initCollisionListener);				

	// PUBLIC METHODS DEFINED INSIDE Physics.cpp - YOU ARE WELCOME TO ADD MORE OR CHANGE WHAT YOU LIKE
	void addCollidableObject(CollidableObject *collidableObjectToAdd);
	void addCollidableObject(CollidableObject* co, float x, float y, int width, int height, float angle, float density, float friction, bool fixedRotation, float gravityScale, bool isCircular);
	void addDynamicRectangle(CollidableObject* co, float centerX, float centerY, int width, int height, float angle, float density, float friction, float gravityScale);
	void addStaticRectangle(int x, int y, float width, float height);
	void removeCollidableObject(CollidableObject *collidableObjectToRemove);
	//void togglePhysics() { activated = !activated; }
	void update(Game *game);
	void addCollidableTiles(TiledLayer *collidableTiles);
	void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);
	void resetWorld()
	{
		b2Body* node = box2DWorld->GetBodyList();

		while (node)
		{
			b2Body* b = node;

			node = node->GetNext();

			box2DWorld->DestroyBody(b);
		}
	}

	void destroyBody(b2Body* body)
	{
		bodiesToDestroy.push_back(body);
	}

	void triggerExplosion(Explosion* explosion)
	{
		explosions.push_back(explosion);
		explosionThisFrame = true;
	}
	void setOffExplosion(b2Vec2 position, int power, int numRays, int radius);

	static Physics* getInstance();
	static float pixelsToMeters(float pixels)
	{
		return pixels/10;
	}
	static float metersToPixels(float meters)
	{
		return meters*10;
	}

private:
	Physics();
	~Physics();
	Physics(Physics const&){};             // copy constructor is private
	Physics& operator=(Physics const&){};  // assignment operator is private

	float scaleValueDownForBox2D(float value)
	{
		value = value/SCALING_FACTOR;
		return value;
	}

	float scaleValueUpFromBox2DForRendering(float value)
	{
		value = value * SCALING_FACTOR;
		return value;
	}

	void setCollisionFilters(CollidableObject* co, b2FixtureDef* body);
};