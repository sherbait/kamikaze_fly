/*	
	Author: Andrew Gratta
			Stony Brook University
			Computer Science Department

	GameObject.h

	Provides update functions that are called every frame in the game loop
*/
#pragma once
#include "stdafx.h"
#include "LuaPlusFramework\LuaPlus.h"
using namespace LuaPlus;

class Game;

class GameObject
{
public:
	GameObject();
	GameObject(const char* script);
	~GameObject();
	static list<GameObject*> activeGameObjects;
private:
	friend class GameStateManager;
	LuaState* luaState;

protected:
	/*
	Called every frame after the world is updated, but before physics is updated
	*/
	virtual void update() = 0;

};