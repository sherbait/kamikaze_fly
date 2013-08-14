/*	
Author: Richard McKenna
Stony Brook University
Computer Science Department

AnimatedSprite.h

This class represents a sprite that can can
be used to animate a game character or object.
*/

#pragma once
#include "stdafx.h"
#include "src\gsm\physics\CollidableObject.h"
#include "src\gsm\physics\PhysicalProperties.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"
#include "src\gui\Viewport.h"
#include "src\game\GameObject.h"
#define DEATH_COUNTDOWN_INITVALUE 54
class AnimatedSprite : public CollidableObject, public GameObject
{
protected:
	// SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
	// OF AN AnimatedSpriteType OBJECT AS STORED IN THE SpriteManager
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

	unsigned int deathCountdown;

	virtual void update() override;

public:
	int					getAlpha()			{ return alpha;				}
	wstring				getCurrentState()	{ return currentState;		}
	unsigned int		getFrameIndex()		{ return frameIndex;		}
	AnimatedSpriteType*	getSpriteType()		{ return spriteType;		}
	unsigned int		getDeathCountdown()	{ return deathCountdown;	}

	// INLINED MUTATOR METHODS
	void setAlpha(int initAlpha)
	{	alpha = initAlpha;						}
	void setSpriteType(AnimatedSpriteType *initSpriteType)
	{	spriteType = initSpriteType;			}

	// METHODS DEFINED IN AnimatedSprite.cpp
	AnimatedSprite();
	~AnimatedSprite();
	void changeFrame();
	unsigned int getCurrentImageID();
	void setCurrentState(wstring newState);
	void updateSprite();
	void affixTightAABBBoundingVolume();
	void correctToTightBoundingVolume();

	//These are only used for the waypoint system. They are only accessed by SpriteManager. 
	//We should probably move them somewhere else, but we'll leave them here for now.
	int key;
	float initX;
	static bool compareByKey ( AnimatedSprite* left, AnimatedSprite* right )
	{ return left->key < right->key; }


private:
	
};