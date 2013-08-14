#pragma once

#include "stdafx.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\AnimatedSprite.h"

class Projectile : public AnimatedSprite
{
public:
	Projectile();
	~Projectile()	{}
	void init();
	int timeToLive;
protected:
	virtual void update() override;
	
};