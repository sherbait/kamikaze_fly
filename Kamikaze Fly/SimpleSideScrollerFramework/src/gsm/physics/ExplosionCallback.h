#pragma once

#include "Box2D\Box2D.h"

class ExplosionCallback : public b2RayCastCallback
{
private:
	b2Fixture* closestFixture;
	b2Vec2 impulsePoint;
public:
	ExplosionCallback(){ closestFixture = NULL;}
	~ExplosionCallback(){ }
	b2Fixture* getClosestFixture()	{ return closestFixture;	}
	b2Vec2& getImplusePoint()		{ return impulsePoint;		}
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) override;
};