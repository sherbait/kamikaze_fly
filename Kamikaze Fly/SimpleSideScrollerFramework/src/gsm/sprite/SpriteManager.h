/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.h

	This class manages all of the sprites in a given game level. Note
	that the player sprite is also managed by this class.
*/

#pragma once
#include "stdafx.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\ai\Bot.h"
#include "src\gsm\ai\BotRecycler.h"
#include "src\gsm\ai\bots\Goal.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\sprite\Projectile.h"
#include "src\gsm\sprite\AnimatedOverlay.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"
#include <stack>

struct waypoint
{
	float x;
	bool activated;
};

class SpriteManager
{
private:
	// NOTE THAT MULTIPLE SPRITES MAY SHARE ARTWORK, SO SPRITE TYPES
	// SPECIFIES A TYPE OF SPRITE, OF WHICH THERE MAY BE MANY INSTANCES
	vector<AnimatedSpriteType*> spriteTypes;

	// THESE ARE THE BOTS IN THE GAME, LIKE ENEMIES, ROCKETS, OR ANYTHING
	// THAT MOVES AROUND AND IS NOT THE PLAYER
	list<Bot*> bots;

	//These are objects in the game that the player must destory, collect, move, etcetera in order to win the
	//game
	list<Goal*> goalObjects;
	Goal* firstGoalObject;

	//These are animated sprites that do not physically interact with the environment. Their purpose is to provide
	//information to the player, or create interesting visual effects.
	list<AnimatedOverlay*> animatedOverlays;

	// AND THIS IS THE PLAYER. AS-IS, WE ONLY ALLOW FOR ONE PLAYER AT A TIME
	AnimatedSprite* player;

	list<Projectile*> projectilePool;
	list<Projectile*> activeProjectiles;
	void destroyProjectile(Projectile* projectile);
	
	WStringTable spriteTypeNameTable;
	list<waypoint*> waypoints;

public:

	SpriteManager()		{player=NULL;}
	~SpriteManager()	{}

	// INLINED ACCESSOR METHODS
	int						getNumberOfSprites()	{ return bots.size();		}
	AnimatedSprite*			getPlayer()				{ return player;			}
	void					setPlayer(AnimatedSprite* p) {player = p;}
	list<Bot*>::iterator	getBotsIterator()		{ return bots.begin();		}
	list<Bot*>::iterator	getEndOfBotsIterator()	{ return bots.end();		}
	list<Goal*>::iterator	getGoalsIterator()		{ return goalObjects.begin();		}
	list<Goal*>::iterator	getEndOfGoalsIterator()	{ return goalObjects.end();		}
	int						getNumberOfGoalObjects() { return goalObjects.size();	}

	// METHODS DEFINED IN SpriteManager.cpp
	void				addBot(Bot *botToAdd);
	void				addGoalObject(Goal* goalObject);
	void				addAnimatedOverlay(AnimatedOverlay* overlay);
	void				addSpriteItemsToRenderList(Game *game);
	unsigned int		addSpriteType(AnimatedSpriteType *spriteTypeToAdd, string name);
	void				addSpriteToRenderList(AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport);
	void				addAnimationToRenderList(AnimatedOverlay *sprite, RenderList *renderList, Viewport *viewport);
	void				clearSprites();
	AnimatedSpriteType* getSpriteType(unsigned int typeIndex);
	AnimatedSpriteType* getSpriteType(wstring name);
	unsigned int		getSpriteTypeIndex(wstring name);
	int					getNumberOfSpritesOfID(int id);

	Bot*				removeBot(Bot *botToRemove);
	void				unloadSprites();
	void				update(Game *game);
	void				reserveProjectiles(int numProjectiles);
	void			createProjectile(float x, float y, float vx, float vy);
};