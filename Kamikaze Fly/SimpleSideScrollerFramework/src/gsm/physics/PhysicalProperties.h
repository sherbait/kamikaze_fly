#pragma once
#include "stdafx.h"
#include "Box2D\Box2D.h"

#define _USE_MATH_DEFINES
#include <math.h>

class PhysicalProperties
{
protected:
	float buoyancy;
	float mass;
	bool  collidable;
	float coefficientOfRestitution;
	float x;
	float y;
	float z;
	float velocityX;
	float velocityY;
	float accelerationX;
	float accelerationY;
	float rotationInRadians;
	bool bodyDisabled;
	float xAtDisabled;
	float yAtDisabled;
	float width;
	float height;

public:
	PhysicalProperties()	{accelerationX = 0; accelerationY = 0; bodyDisabled=false;}
	~PhysicalProperties()	{}

	b2Body* rigidBody;

	// INLINED METHODS
	float getBuoyancy()						{ return buoyancy;				}
	float getMass()							{ return mass;					}
	float getCoefficientOfRestitution()		{ return coefficientOfRestitution;	}
	bool isCollidable()						{ return rigidBody->IsActive();			}
	float getX()							
	{ 
		if(bodyDisabled)
			return xAtDisabled;

		return metersToPixels(rigidBody->GetPosition().x) - width/2;	
	}
	float getY()							
	{ 
		if(bodyDisabled)
			return yAtDisabled;

		return -metersToPixels(rigidBody->GetPosition().y) - height/2;	
	}
	float getZ()							{ return z;						}
	float getVelocityX()					
	{ 
		if(bodyDisabled)
			return 0;

		return metersToPixels(rigidBody->GetLinearVelocity().x);				
	}
	float getVelocityY()					
	{
		if(bodyDisabled)
			return 0;

		return -metersToPixels(rigidBody->GetLinearVelocity().y);				
	}
	float getAccelerationX()				{ return accelerationX;			}
	float getAccelerationY()				{ return accelerationY;			}
	void setBuoyancy(float initBuoyancy)	{ buoyancy = initBuoyancy;		}
	void setMass(float initMass)			{ mass = initMass;				}
	void setCofficientOfRestitution(float cR) { coefficientOfRestitution = cR; }
	void setCollidable(bool initCollidable)	{ rigidBody->SetActive(initCollidable);	}
	void setX(float initX)					{ x = initX;					}
	void incX(float xToInc)					{ x += xToInc;					}
	void setY(float initY)					{ y = initY;					}
	void incY(float yToInc)					{ y += yToInc;					}
	void setZ(float initZ)					{ z = initZ;					}
	void setWidth(float width)				{ this->width = width;			}
	void setHeight(float height)			{ this->height = height;		}
	float getRotationInRadians()			{ return rigidBody->GetAngle();	}
	void disableBox2DBody(bool disabled)
	{
		bodyDisabled = disabled;
		xAtDisabled = metersToPixels(rigidBody->GetPosition().x);
		yAtDisabled = -metersToPixels(rigidBody->GetPosition().y);
	}
	bool isDisabled()
	{
		return bodyDisabled;
	}
	b2Body* getBody()
	{
		return rigidBody;
	}

	float getHeight()
	{ return height; }
	float getWidth()
	{ return width; }

	int round(float realNum)
	{
		return (int)floor(realNum + 0.5f);
	}

	void setRotation(float degrees)
	{
		if(bodyDisabled)
			return;

		rigidBody->SetTransform(rigidBody->GetPosition(), (M_PI/180) * (degrees));
	}

	void setPosition(float initX, float initY)
	{
		rigidBody->SetTransform(b2Vec2(pixelsToMeters(initX), -pixelsToMeters(initY)), rigidBody->GetAngle());
	}

	void setVelocity(float initVx, float initVy)
	{
		rigidBody->SetLinearVelocity(b2Vec2(pixelsToMeters(initVx), -pixelsToMeters(initVy)));
	}

	void incVelocity(float incVx, float incVy)
	{ 
		velocityX += incVx;
		velocityY += incVy;
	}
	void setAccelerationX(float initAx)		{ accelerationX = initAx;		}
	void setAccelerationY(float initAy)		{ accelerationY = initAy;		}

	float calcTotalVelocity()
	{
		float xSquared = x * x;
		float ySquared = y * y;
		float totalVelocity = xSquared + ySquared;
		totalVelocity = sqrtf(totalVelocity);
		return totalVelocity;
	}

	void applyAcceleration()
	{
		velocityX += accelerationX;
		velocityY += accelerationY;
	}

	void applyForce(float x, float y)
	{
		rigidBody->ApplyForceToCenter(b2Vec2(x,y));
	}

	float pixelsToMeters(float pixels)
	{
		return pixels/10;
	}
	float metersToPixels(float meters)
	{
		return meters*10;
	}
};