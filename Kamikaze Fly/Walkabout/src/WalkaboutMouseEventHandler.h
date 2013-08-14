#pragma once

#include "src\game\Game.h"
#include "src\input\MouseEventHandler.h"

class WalkaboutMouseEventHandler : public MouseEventHandler
{
public:
	WalkaboutMouseEventHandler() {}
	~WalkaboutMouseEventHandler() {}
	void handleMousePressEvent(Game *game, int mouseX, int mouseY);
	void handleMouseMoveEvent(Game *game, int mouseX, int mouseY);
};