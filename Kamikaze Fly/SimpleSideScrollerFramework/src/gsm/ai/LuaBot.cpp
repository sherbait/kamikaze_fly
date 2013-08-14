#include "stdafx.h"
#include "src\game\Game.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gsm\physics\Physics.h"
#include "src\gsm\ai\LuaBot.h"

LuaBot::LuaBot(const char* script)
{
	luaState = LuaState::Create();
	int result = luaState->DoFile(script);
	LuaObject globals = luaState->GetGlobals();
	AnimatedSprite* testBot = this;

	globals.RegisterDirect("setState",*this, &LuaBot::setState);
	globals.RegisterDirect("distanceToPlayer", *this, &LuaBot::distanceToPlayer);
	globals.RegisterDirect("playerX", *this, &LuaBot::playerX);
	globals.RegisterDirect("playerY", *this, &LuaBot::playerY);
	globals.RegisterDirect("moveToPlayer", *this, &LuaBot::moveToPlayer);

	globals.RegisterDirect("setAlpha",*testBot, &AnimatedSprite::setAlpha);

	PhysicalProperties* pp = testBot->getPhysicalProperties();
	globals.RegisterDirect("getX",*pp, &PhysicalProperties::getX);
	globals.RegisterDirect("getY",*pp, &PhysicalProperties::getY);
	globals.RegisterDirect("getVx",*pp, &PhysicalProperties::getVelocityX);
	globals.RegisterDirect("getVy",*pp, &PhysicalProperties::getVelocityY);
	globals.RegisterDirect("applyForce",*pp, &PhysicalProperties::applyForce);
	globals.RegisterDirect("setVelocity",*pp, &PhysicalProperties::setVelocity);

	collisionType = CollisionType::BOT;
}

void LuaBot::think(Game* game)
{
	LuaFunction<void> updateFunc = luaState->GetGlobal("update");
	if (isDying || isDead)
	{
		return;
	}
	updateFunc();
}

void LuaBot::setState(const char* state)
{
	setCurrentState(wstring(state, state + strlen(state)));
}

float LuaBot::distanceToPlayer()
{
	AnimatedSprite* player = Game::getInstance()->getGSM()->getSpriteManager()->getPlayer();
	float px = player->getPhysicalProperties()->getX();
	float py = player->getPhysicalProperties()->getY();
	float dx = pp.getX() - px;
	float dy = pp.getY() - py;
	float f = fabsf(dx*dx) + fabsf(dy*dy);
	return sqrtf(f);
}

float LuaBot::playerX()
{
	return Game::getInstance()->getGSM()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getX();
}

float LuaBot::playerY()
{
	return Game::getInstance()->getGSM()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getY();
}

void LuaBot::moveToPlayer(float speed)
{
	float dx = playerX()-pp.getX();
	float dy = playerY()-pp.getY();
	float length = sqrtf(dx*dx + dy*dy);
	dx /= length;
	dy /= length;
	pp.setVelocity(dx*speed,dy*speed);
}