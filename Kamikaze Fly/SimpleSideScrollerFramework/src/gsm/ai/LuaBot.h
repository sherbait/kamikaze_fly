#pragma once

#include "stdafx.h"
#include "src\game\Game.h"
#include "src\gsm\ai\Bot.h"
#include "LuaPlusFramework\LuaPlus.h"

class LuaBot : public Bot
{
public:
	LuaBot(const char* script);
	~LuaBot()	{}

	void think(Game* game);
	void setState(const char* state);
	float distanceToPlayer();

private:
	LuaState* luaState;
	float playerX();
	float playerY();
	void moveToPlayer(float speed);
	//LuaFunction<void> updateFunc;
};