#include "stdafx.h"
#include "src\game\Game.h"
#include "src\gsm\ai\bots\Block.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gsm\physics\Physics.h"


Block::Block(){
}

/*
	clone - this method makes another Block object, but does
	not completely initialize it with similar data to this. Most of the 
	object, like velocity and position, are left uninitialized.
*/
Bot* Block::clone()
{
	Block *botClone = new Block();
	return botClone;
}

/*
	initBot - this initialization method sets up the basic bot
	properties, but does not setup its velocity.
*/
void Block::initBot(	unsigned int initMin,
									unsigned int initMax,
									unsigned int initMaxVelocity)
{

}


/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void Block::think(Game *game)
{

}