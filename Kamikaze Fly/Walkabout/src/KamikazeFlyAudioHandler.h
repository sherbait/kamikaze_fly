/*
	Author: Dinia Gepte
			Stony Brook University
			Computer Science Department
*/

#pragma once

#include "stdafx.h"
#include "src\audio\AudioHandler.h"
#include "src\gsm\state\GameState.h"
#include "src\gsm\ai\Bot.h"

class Game;

const float PLAYER_TO_BOT_RANGE = 300.0f;

class KamikazeFlyAudioHandler: public AudioHandler
{
private:
	enum channels {CH_MAIN, CH_INGAME, CH_CLICK, CH_PLAYER, CH_BLAST_BOT, CH_BLAST_PLAYER,
				   CH_BOT1, CH_BOT2, CH_BOT3, CH_BOT4, CH_BOT5};	/* For maximum number of channels: See AudioManager.h */
	
	// MEMBERS
	map<int, Bot*> botChannels;

	// METHODS
	const char* getFilePath(int spriteTypeID);
	bool isWithinRange(float x1, float y1, float x2, float y2);
	bool isBotInBotChannels(Bot *bot);
	bool botHasSFX(Bot *bot);
	int	 getChannelInBotChannels(Bot *bot);
	void stopAllChannels(Game *game);
	void toggleAllChannels(Game *game);
public:
	KamikazeFlyAudioHandler()	{}
	~KamikazeFlyAudioHandler()	{}

	// INHERITED METHODS
	void handleGameEvents(Game *game);
	void handleButtonEvents(Game *game);
};