#pragma once

#include "stdafx.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gui\OverlayImage.h"

#define HEALTH 5
#define AMMO 5

class Player : public AnimatedSprite
{
public:
	Player();
	~Player()
	{
		//delete healthImages;
		//delete ammoImages;
	}
	void damage(){hit = true;}
	void decrementAmmo(){ ammo--;}
	void setAmmo(int ammo){this->ammo = ammo;}
	int getAmmo() {return ammo;}

	void virtual handlePreSolve(b2Contact* contact, CollidableObject* otherCollidableObject) override;
	void virtual handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse,CollidableObject* otherCollidableObject) override;

protected:
	int health;
	int lives;
	int ammo;
	int timer;
	bool hit;
	bool dyingLastFrame;
	OverlayImage* healthImages[HEALTH];
	OverlayImage* ammoImages[AMMO];
	OverlayImage* life1;
	OverlayImage* life2;
	void update();
};