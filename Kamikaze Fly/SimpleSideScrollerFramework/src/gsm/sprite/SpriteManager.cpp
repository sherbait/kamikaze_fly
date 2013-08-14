/*	
Author: Richard McKenna
Stony Brook University
Computer Science Department

SpriteManager.cpp

See SpriteManager.h for a class description.
*/

#pragma once
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\gsm\physics\PhysicalProperties.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\sprite\AnimatedOverlay.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\gsm\state\GameStateManager.h"

/*
addSpriteToRenderList - This method checks to see if the sprite
parameter is inside the viewport. If it is, a RenderItem is generated
for that sprite and it is added to the render list.
*/
void SpriteManager::addSpriteToRenderList(AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	PhysicalProperties *pp = sprite->getPhysicalProperties();

	// IS THE SPRITE VIEWABLE?
	if (viewport->areWorldCoordinatesInViewport(	
		pp->getX(),
		pp->getY(),
		spriteType->getTextureWidth(),
		spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		float x = pp->getX();
		float y = pp->getY();
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		renderList->addRenderItem(	sprite->getCurrentImageID(),
			pp->round(pp->getX()-viewport->getViewportX()),
			pp->round(pp->getY()-viewport->getViewportY()),
			pp->round(pp->getZ()),
			sprite->getAlpha(),
			spriteType->getTextureWidth(),
			spriteType->getTextureHeight(),
			pp->getRotationInRadians());
	}
}

void SpriteManager::addAnimationToRenderList(AnimatedOverlay *animation, RenderList *renderList, Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = animation->getSpriteType();

	if (viewport->areWorldCoordinatesInViewport(	
		animation->getX(),
		animation->getY(),
		spriteType->getTextureWidth(),
		spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		float x = animation->getX();
		float y = animation->getY();
		RenderItem itemToAdd;
		itemToAdd.id = animation->getFrameIndex();
		renderList->addRenderItem(animation->getCurrentImageID(),
			animation->round(animation->getX()-viewport->getViewportX()),
			animation->round(animation->getY()-viewport->getViewportY()),
			animation->round(animation->getZ()),
			animation->getAlpha(),
			spriteType->getTextureWidth(),
			spriteType->getTextureHeight(),
			0);
	}
}

/*
addSpriteItemsToRenderList - This method goes through all of the sprites,
including the player sprite, and adds the visible ones to the render list.
This method should be called each frame.
*/
void SpriteManager::addSpriteItemsToRenderList(	Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		//Add the goal objects
		list<Goal*>::iterator goalIterator;
		goalIterator = goalObjects.begin();
		while (goalIterator != goalObjects.end())
		{			
			Goal *goal = (*goalIterator);
			addSpriteToRenderList(goal, renderList, viewport);
			goalIterator++;
		}

		// NOW ADD THE REST OF THE SPRITES
		list<Bot*>::iterator botIterator;
		botIterator = bots.begin();
		while (botIterator != bots.end())
		{			
			Bot *bot = (*botIterator);
			addSpriteToRenderList(bot, renderList, viewport);
			botIterator++;
		}

		// ADD THE PLAYER SPRITE
		addSpriteToRenderList(player, renderList, viewport);

		//Add the animated overlays
		list<AnimatedOverlay*>::iterator animationIterator;
		animationIterator = animatedOverlays.begin();
		while(animationIterator != animatedOverlays.end())
		{
			AnimatedOverlay* animation = *animationIterator;

			addAnimationToRenderList(animation, renderList, viewport);

			animationIterator++;
		}

		list<Projectile*>::iterator projectileIterator;
		projectileIterator = activeProjectiles.begin();
		while(projectileIterator != activeProjectiles.end())
		{
			Projectile* p = *projectileIterator;

			addSpriteToRenderList(p, renderList, viewport);

			projectileIterator++;
		}
	}
}

/*
addSprite - This method is for adding a new sprite to 
this sprite manager. Once a sprite is added it can be 
scheduled for rendering.
*/
void SpriteManager::addBot(Bot *botToAdd)
{
	bots.push_back(botToAdd);
}

void SpriteManager::addGoalObject(Goal *goalObject)
{
	goalObjects.push_back(goalObject);
	goalObject->initX = goalObject->getPhysicalProperties()->getX();
	waypoint* newWaypoint = new waypoint();
	newWaypoint->activated = false;
	newWaypoint->x = goalObject->getPhysicalProperties()->getX();
	waypoints.push_back(newWaypoint);
}

void SpriteManager::addAnimatedOverlay(AnimatedOverlay *overlay)
{
	animatedOverlays.push_back(overlay);
}

/*
addSpriteType - This method is for adding a new sprite
type. Note that one sprite type can have many sprites. For
example, we may say that there may be a "Bunny" type of
sprite, and specify properties for that type. Then there might
be 100 different Bunnies each with their own properties, but that
share many things in common according to what is defined in
the shared sprite type object.
*/
unsigned int SpriteManager::addSpriteType(AnimatedSpriteType *spriteTypeToAdd, string name)
{
	spriteTypes.push_back(spriteTypeToAdd);
	spriteTypeNameTable.putWStringInTable(wstring(name.begin(),name.end()));
	return spriteTypes.size()-1;
}

unsigned int SpriteManager::getSpriteTypeIndex(wstring name)
{
	return spriteTypeNameTable.getIndexOfWString(name);
}

AnimatedSpriteType* SpriteManager::getSpriteType(wstring name)
{
	int index = getSpriteTypeIndex(name);
	return spriteTypes.at(index);
}

int	SpriteManager::getNumberOfSpritesOfID(int id)
{
	int count = 0;

	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		if (bot->getSpriteType()->getSpriteTypeID() == id)
			count++;
		botIterator++;
	}

	return count;
}

/*
clearSprites - This empties all of the sprites and sprite types.
*/
void SpriteManager::clearSprites()
{
	bots.clear();
	goalObjects.clear();
	animatedOverlays.clear();
	activeProjectiles.clear();
	projectilePool.clear();
	waypoints.clear();
	delete player;
}

/*
getSpriteType - This gets the sprite type object that corresponds
to the index argument.
*/
AnimatedSpriteType* SpriteManager::getSpriteType(unsigned int typeIndex)
{
	if (typeIndex < spriteTypes.size())
		return spriteTypes.at(typeIndex);
	else
		return NULL;
}

/*
unloadSprites - This method removes all artwork from memory that
has been allocated for game sprites.
*/
void SpriteManager::unloadSprites()
{
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT
}

void SpriteManager::createProjectile(float x, float y, float vx, float vy)
{
	if (projectilePool.empty())
	{
		return;
	}
	Projectile* p = projectilePool.back();
	PhysicalProperties* phys = p->getPhysicalProperties();
	phys->setPosition(x,y);
	phys->setCollidable(true);
	p->setIsDead(false);
	p->timeToLive = 30;
	phys->setVelocity(vx,vy);
	p->setAlpha(255);
	activeProjectiles.push_back(p);
	projectilePool.pop_back();
}

void SpriteManager::destroyProjectile(Projectile* p)
{
	p->getPhysicalProperties()->setCollidable(false);
	p->setIsDead(true);
	p->setAlpha(0);
	activeProjectiles.remove(p);
	projectilePool.push_back(p);
}

Bot* SpriteManager::removeBot(Bot *botToRemove)
{
	return NULL;
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT
}

/*
update - This method should be called once per frame. It
goes through all of the sprites, including the player, and calls their
update method such that they may update themselves.
*/
void SpriteManager::update(Game *game)
{
	// UPDATE THE PLAYER SPRITE
	if(!player->getIsDead())
	{
		player->updateSprite();
	}
	//Update the goal objects
	list<Goal*>::iterator goalIterator;
	goalIterator = goalObjects.begin();
	list<Goal*> completedGoals;
	while(goalIterator != goalObjects.end())
	{
		Goal* goalObject = *goalIterator;
		if(!goalObject->getIsDead())
		{
			goalObject->updateSprite();
			goalObject->think(game);
		}
		else
		{
			completedGoals.push_back(goalObject);
		}

		goalIterator++;
	}

	goalIterator = completedGoals.begin();
	list<waypoint*>::iterator waypointIterator;
	while(goalIterator != completedGoals.end())
	{
		Goal* completedGoalObject = *goalIterator;

		if(completedGoalObject == *(goalObjects.begin()))
		{
			waypointIterator = waypoints.begin();
			while(waypointIterator != waypoints.end())
			{
				waypoint* nextWaypoint = *waypointIterator;
				waypoint* prevWaypoint = NULL;
				if(nextWaypoint->x == completedGoalObject->initX)
				{
					nextWaypoint->activated = true;
					waypointIterator++;
					bool continueSearching = true;
					while(continueSearching && waypointIterator!=waypoints.end())
					{
						prevWaypoint = nextWaypoint;
						nextWaypoint = *waypointIterator;
						if(!nextWaypoint->activated)
						{
							continueSearching = false;
						}
						else
						{
							waypointIterator++;
						}
					}

					if(prevWaypoint)
					{
						game->setWaypoint(prevWaypoint->x,0);
					}
					break;
				}
				waypointIterator++;
			}
		}
		else
		{
			waypointIterator = waypoints.begin();
			waypoint* nextWaypoint = *waypointIterator;
			while(waypointIterator != waypoints.end())
			{
				if(nextWaypoint->x == completedGoalObject->initX)
				{
					nextWaypoint->activated = true;
					break;
				}
				waypointIterator++;
				nextWaypoint = *waypointIterator;
			}
		}

		goalObjects.remove(completedGoalObject);
		goalIterator++;
	}

	completedGoals.clear();

	//goalObjects.sort(AnimatedSprite::compareByKey);

	//Update the animated overlays
	list<AnimatedOverlay*>::iterator animationIterator;
	animationIterator = animatedOverlays.begin();
	list<AnimatedOverlay*> finishedAnimations;
	while(animationIterator != animatedOverlays.end())
	{
		AnimatedOverlay* animation = *animationIterator;

		if(animation->getMarkedForRemoval())
		{
			finishedAnimations.push_back(animation);
		}
		else
		{
			animation->updateSprite();
		}

		animationIterator++;
	}

	animationIterator = finishedAnimations.begin();
	while(animationIterator != finishedAnimations.end())
	{
		AnimatedOverlay* finishedAnimation = *animationIterator;
		animatedOverlays.remove(finishedAnimation);

		animationIterator++;
	}
	finishedAnimations.clear();

	// NOW UPDATE THE REST OF THE SPRITES
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	list<Bot*> deadBots;
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		if(!bot->getIsDead())
		{
			bot->think(game);
			bot->updateSprite();
		}
		else
		{
			deadBots.push_back(bot);
		}
		botIterator++;
	}

	list<Bot*>::iterator deadBotIterator = deadBots.begin();
	while (deadBotIterator != deadBots.end())
	{
		Bot *bot = (*deadBotIterator);
		bots.remove(bot);
		deadBotIterator++;
	}
	deadBots.clear();

	list<Projectile*>::iterator projectileIterator = activeProjectiles.begin();
	list<Projectile*> deadProjectiles;
	while(projectileIterator != activeProjectiles.end())
	{
		Projectile* p = *projectileIterator;
		if (p->getIsDead())
		{
			deadProjectiles.push_back(p);
		}

		projectileIterator++;
	}

	list<Projectile*>::iterator deadProjectileIterator = deadProjectiles.begin();
	while (deadProjectileIterator != deadProjectiles.end())
	{
		Projectile* p = *deadProjectileIterator;
		destroyProjectile(p);
		deadProjectileIterator++;
	}
}

void SpriteManager::reserveProjectiles(int numProjectiles)
{
	if (!projectilePool.empty())
	{
		return;
	}
	for (int i = 0; i < numProjectiles; i++)
	{
		Projectile* p = new Projectile();
		p->init();
		projectilePool.push_back(p);
	}
}