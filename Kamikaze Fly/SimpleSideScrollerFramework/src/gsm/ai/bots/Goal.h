#pragma once

#include "stdafx.h"
#include "src\gsm\ai\bots\Block.h"
#include "src\gui\OverlayImage.h"

class Goal : public Block
{
public:
	Goal(){
		setCollisionType(CollisionType::GOAL);
		maxVelocity = 10;
		previousVelocity = 0;
	}
	virtual void think(Game* game) override;

protected:
	int maxVelocity;
	float previousVelocity;
};