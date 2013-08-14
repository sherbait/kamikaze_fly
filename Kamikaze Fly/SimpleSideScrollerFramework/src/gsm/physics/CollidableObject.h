#pragma once
#include "stdafx.h"
#include "src\gsm\physics\AABB.h"
#include "src\gsm\physics\PhysicalProperties.h"

enum CollisionType
{
	BOT = 0x0001,
	PLAYER = 0x0002,
	PROJECTILE = 0x0004,
	BLOCK = 0x0008,
	TILE = 0x0010,
	GOAL = 0x0020
};

class CollidableObject
{
protected:
	AABB boundingVolume;
	bool currentlyCollidable;
	AABB sweptShape;
	PhysicalProperties pp;
	bool isDead;
	bool isDying;
	bool isFalling;
	CollisionType collisionType;
public:
	CollidableObject()	{ isDead = false; isDying = false; isFalling = false;}
	CollidableObject(CollisionType type){isDying = false; collisionType = type;}
	~CollidableObject()	{}

	// CollidableObject.cpp METHODS
	void updateSweptShape(float percentageOfFrameTimeRemaining);

	// INLINED METHODS
	AABB*				getBoundingVolume()			{ return &boundingVolume;		}
	AABB*				getSweptShape()				{ return &sweptShape;			}
	PhysicalProperties* getPhysicalProperties()		{ return &pp;					}
	bool getIsDead()								{ return isDead;				}
	bool getIsDying()								{ return isDying;				}
	bool getIsFalling()								{ return isFalling;				}
	CollisionType getCollisionType()				{return collisionType;			}

	void setIsDead(bool dead)
	{ 
		isDead = dead; 
	}
	void setIsDying(bool dying)
	{ 
		isDying = dying; 
	}
	void setIsFalling(bool falling)
	{ 
		isFalling = falling; 
	}
	
	void setCollisionType(CollisionType type)
	{
		collisionType = type;
	}

	void virtual handleBeginContact(b2Contact* contact, CollidableObject* otherCollidableObject){}
	void virtual handleEndContact(b2Contact* contact, CollidableObject* otherCollidableObject){}
	void virtual handlePreSolve(b2Contact* contact, CollidableObject* otherCollidableObject){}
	void virtual handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse,CollidableObject* otherCollidableObject){}
};