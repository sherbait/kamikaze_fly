#pragma once
#include "stdafx.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"
#include "src\gui\Viewport.h"
#include "src\game\GameObject.h"
#include "src\gsm\physics\CollidableObject.h"
class AnimatedOverlay
{
protected:
	// SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
	// OF AN AnimatedOverlayType OBJECT AS STORED IN THE SpriteManager
	AnimatedSpriteType *spriteType;

	// TRANSPARENCY/OPACITY
	int alpha;

	// THE "current" STATE DICTATES WHICH ANIMATION SEQUENCE 
	// IS CURRENTLY IN USE, BUT IT MAP ALSO BE USED TO HELP
	// WITH OTHER GAME ACTIVITIES, LIKE PHYSICS
	wstring currentState;

	// THE INDEX OF THE CURRENT FRAME IN THE ANIMATION SEQUENCE
	// NOTE THAT WE WILL COUNT BY 2s FOR THIS SINCE THE VECTOR
	// THAT STORES THIS DATA HAS (ID,DURATION) TUPLES
	unsigned int frameIndex;

	// USED TO ITERATE THROUGH THE CURRENT ANIMATION SEQUENCE
	unsigned int animationCounter;

	float x;
	float y;
	float z;

	bool remove;

	CollidableObject* objectToFollow;

	void update();

public:
	// INLINED ACCESSOR METHODS
	int					getAlpha()			{ return alpha;				}
	wstring				getCurrentState()	{ return currentState;		}
	unsigned int		getFrameIndex()		{ return frameIndex;		}
	AnimatedSpriteType*	getSpriteType()		{ return spriteType;		}
	float				getZ()				{ return z;					}
	bool				getMarkedForRemoval(){return remove;			}
	float getX()
	{
		if(objectToFollow)
		{
			return objectToFollow->getPhysicalProperties()->getX();
		}

		return x;					
	}
	float getY()				
	{ 
		if(objectToFollow)
		{
			return objectToFollow->getPhysicalProperties()->getY() - spriteType->getTextureHeight() - 5;
		}
		return y;					
	}
	int round(float realNum)
	{
		return (int)floor(realNum + 0.5f);
	}

	// INLINED MUTATOR METHODS
	void setAlpha(int initAlpha)
	{	alpha = initAlpha;						}
	void setSpriteType(AnimatedSpriteType *initSpriteType)
	{	spriteType = initSpriteType;			}
	void setX(float x)
	{ this->x =x; }
	void setY(float y)
	{ this->y =y; }
	void setZ(float z)
	{ this->z =z; }
	void markForRemoval()
	{ remove = true; }

	void followCollidableObject(CollidableObject* objectToFollow)
	{ this->objectToFollow = objectToFollow; }


	// METHODS DEFINED IN AnimatedOverlay.cpp
	AnimatedOverlay();
	~AnimatedOverlay();
	void changeFrame();
	unsigned int getCurrentImageID();
	void setCurrentState(wstring newState);
	void updateSprite();
};