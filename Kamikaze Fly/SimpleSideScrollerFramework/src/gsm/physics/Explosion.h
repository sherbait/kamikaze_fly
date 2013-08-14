#pragma once

#include "Box2D\Box2D.h"

class Explosion
{
public:
	b2Vec2 explosionEpicenter;
	float blastRadius;
	int numOfRays;
	float blastPower;
};