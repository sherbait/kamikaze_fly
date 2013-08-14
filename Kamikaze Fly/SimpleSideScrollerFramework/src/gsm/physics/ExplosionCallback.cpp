#include "Box2D\Box2D.h"
#include "ExplosionCallback.h"

float32 ExplosionCallback::ReportFixture(b2Fixture* fixture, 
										 const b2Vec2& point, 
										 const b2Vec2& normal, 
										 float32 fraction)
{
	closestFixture = fixture;
	impulsePoint = point;
	return fraction;
}