/*
	Author: Andrew Gratta
			Stony Brook University
			Computer Science Department

	GameObject.cpp
*/

#include "stdafx.h"
#include "src\game\GameObject.h"

list<GameObject*> GameObject::activeGameObjects;

GameObject::GameObject()
{
	activeGameObjects.push_back(this);
	luaState = nullptr;
}

GameObject::GameObject(const char* script)
{
	activeGameObjects.push_back(this);
	luaState = LuaState::Create();
	luaState->DoFile(script);
}

GameObject::~GameObject()
{
	activeGameObjects.remove(this);
}