/*
	Author: Dinia Gepte
			Stony Brook University
			Computer Science Department
*/

#include "stdafx.h"

// KamikazeFlyGame includes
#include "src\WalkaboutGame.h"

// Game object includes
#include "src\audio\AudioManager.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\KamikazeFlyAudioHandler.h"


/*
	Called every frame. This handles all game audio events.
*/
void KamikazeFlyAudioHandler::handleGameEvents(Game *game)
{
	GameStateManager *gsm = game->getGSM();
	AudioManager *audio = game->getAudioManager();
	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
	SpriteManager *spriteManager = gsm->getSpriteManager();

	GameState gameState = gsm->getCurrentGameState();
	wstring currentLevelName = game->getCurrentLevelFileName();

	//// BACKGROUND AUDIO ////

	// The application starts. Play a background music.
	if (gameState == GS_SPLASH_SCREEN && !audio->isChannelPlaying(CH_MAIN))
	{
		audio->playSound(audio->getSound(C_MAIN_AUDIO_PATH), audio->getChannel(CH_MAIN));
	}
	// The application has transitioned to the actual game. Play an appropriate music.
	if (gameState == GS_GAME_IN_PROGRESS && audio->isChannelPlaying(CH_MAIN))
	{
		// Stop the main background music. We don't need it anymore.
		audio->stopChannel(CH_MAIN);

		// Play an appropriate audio for each level.
		if (currentLevelName.compare(W_LEVEL_1_NAME) == 0)
		{
			audio->playSound(audio->getSound(C_LEVEL_1_AUDIO_PATH), audio->getChannel(CH_INGAME));
			audio->setChannelVolume(CH_INGAME, 0.2);
		}
		else if (currentLevelName.compare(W_LEVEL_2_NAME) == 0)
		{
			audio->playSound(audio->getSound(C_LEVEL_2_AUDIO_PATH), audio->getChannel(CH_INGAME));
			audio->setChannelVolume(CH_INGAME, 0.2);
		}
		else if (currentLevelName.compare(W_LEVEL_3_NAME) == 0)
		{
			audio->playSound(audio->getSound(C_LEVEL_3_AUDIO_PATH), audio->getChannel(CH_INGAME));
			audio->setChannelVolume(CH_INGAME, 0.2);
		}
	}
	// The player went back to the main menu. Play the main background music again.
	if (gameState == GS_MAIN_MENU && audio->isChannelPlaying(CH_INGAME))
	{
		// Stop the level music and any in-game sounds. We don't need them anymore.
		stopAllChannels(game);

		// And play the main background music.
		audio->playSound(audio->getSound(C_MAIN_AUDIO_PATH), audio->getChannel(CH_MAIN));
	}

	//// GAME WORLD AUDIO ////
	if (gameState == GS_GAME_IN_PROGRESS)
	{
		// PLAYER PROPERTIES
		float playerX = player->getPhysicalProperties()->getX();
		float playerY = player->getPhysicalProperties()->getY();

		// PLAYER SOUNDS //

		// The player has just respawned in the game world.
		if (!audio->isChannelPlaying(CH_PLAYER))
		{
			audio->playSound(audio->getSound(C_FLY_LONG_AUDIO_PATH), audio->getChannel(CH_PLAYER));
		}
		// The player has collided with something, and explodes.
		if (player->getDeathCountdown() == DEATH_COUNTDOWN_INITVALUE-1)
		{
			audio->playSound(audio->getSound(C_BLAST_SM_AUDIO_PATH), audio->getChannel(CH_BLAST_PLAYER));
			audio->setChannelVolume(CH_BLAST_PLAYER, 0.4f);
		}

		// BOT SOUNDS //
		list<Bot*>::iterator botIterator;
		botIterator = spriteManager->getBotsIterator();
		while (botIterator != spriteManager->getEndOfBotsIterator())
		{
			Bot *bot = (*botIterator);
			float botX = bot->getPhysicalProperties()->getX();
			float botY = bot->getPhysicalProperties()->getY();
			int botId = bot->getSpriteType()->getSpriteTypeID();

			// The bot explodes.
			if (bot->getDeathCountdown() == DEATH_COUNTDOWN_INITVALUE-1)
			{
				audio->playSound(audio->getSound(C_BLAST_SM_AUDIO_PATH), audio->getChannel(CH_BLAST_BOT));
				audio->setChannelVolume(CH_BLAST_BOT, 0.4f);
			}
			// The bot is near to the right of the player.
			else if (isWithinRange(playerX, playerY, botX, botY) && (botX > playerX) && (botHasSFX(bot)))
			{
				// The bot in range hasn't played a sound effect.
				if (!isBotInBotChannels(bot))
				{
					// We only allow 5 bot sounds at the same time.
					if (!audio->isChannelPlaying(CH_BOT1))
					{
						audio->playSound(audio->getSound(getFilePath(botId)), audio->getChannel(CH_BOT1));
						audio->setChannelVolume(CH_BOT1, 0.4f);
						botChannels.insert(pair<int, Bot*>(CH_BOT1, bot));
					}	
					else if (!audio->isChannelPlaying(CH_BOT2))
					{
						audio->playSound(audio->getSound(getFilePath(botId)), audio->getChannel(CH_BOT2));
						audio->setChannelVolume(CH_BOT2, 0.4f);
						botChannels.insert(pair<int, Bot*>(CH_BOT2, bot));
					}
					else if (!audio->isChannelPlaying(CH_BOT3))
					{
						audio->playSound(audio->getSound(getFilePath(botId)), audio->getChannel(CH_BOT3));
						audio->setChannelVolume(CH_BOT3, 0.4f);
						botChannels.insert(pair<int, Bot*>(CH_BOT3, bot));
					}
					else if (!audio->isChannelPlaying(CH_BOT4))
					{
						audio->playSound(audio->getSound(getFilePath(botId)), audio->getChannel(CH_BOT4));
						audio->setChannelVolume(CH_BOT4, 0.4f);
						botChannels.insert(pair<int, Bot*>(CH_BOT4, bot));
					}
					else if (!audio->isChannelPlaying(CH_BOT5))
					{
						audio->playSound(audio->getSound(getFilePath(botId)), audio->getChannel(CH_BOT5));
						audio->setChannelVolume(CH_BOT5, 0.4f);
						botChannels.insert(pair<int, Bot*>(CH_BOT5, bot));
					}
				}	
			}
			// Stop playing the bot audio if it gets out of screen
			else if (botX < game->getGUI()->getViewport()->getViewportX())
			{
				if (isBotInBotChannels(bot))
				{
					// Stop the sound from playing.
					audio->stopChannel(getChannelInBotChannels(bot));

					// Remove it from the map.
					botChannels.erase(getChannelInBotChannels(bot));
				}
			}

			botIterator++;
		}
	}
}

void KamikazeFlyAudioHandler::stopAllChannels(Game *game)
{
	AudioManager *audio = game->getAudioManager();
	audio->stopChannel(CH_MAIN);
	audio->stopChannel(CH_INGAME);
	audio->stopChannel(CH_PLAYER);
	audio->stopChannel(CH_BLAST_PLAYER);
	audio->stopChannel(CH_BLAST_BOT);
	audio->stopChannel(CH_BOT1);
	audio->stopChannel(CH_BOT2);
	audio->stopChannel(CH_BOT3);
	audio->stopChannel(CH_BOT4);
	audio->stopChannel(CH_BOT5);
}

void KamikazeFlyAudioHandler::toggleAllChannels(Game *game)
{
	AudioManager *audio = game->getAudioManager();
	audio->toggleChannel(CH_MAIN);
	audio->toggleChannel(CH_INGAME);
	audio->toggleChannel(CH_PLAYER);
	audio->toggleChannel(CH_BLAST_PLAYER);
	audio->toggleChannel(CH_BLAST_BOT);
}

void KamikazeFlyAudioHandler::handleButtonEvents(Game *game)
{
	AudioManager *audio = game->getAudioManager();
	audio->playSound(audio->getSound(C_BUTTON_CLICK_AUDIO_PATH), audio->getChannel(CH_CLICK));
}

/*
	This returns the audio filepath of the sound associated with a sprite from sprite_type_list.xml.
	This should be update along with the xml file! Note that the cases can be replaced using 
	spriteManager->getSpriteTypeIndex().
*/
const char* KamikazeFlyAudioHandler::getFilePath(int id)
{
	switch (id)
	{
	case 0:	// bee
		return C_BEE_AUDIO_PATH;
		break;
	case 1: // butterfly
		return C_BUTTERFLY_AUDIO_PATH;
		break;
	case 2: // fly_drone
		return C_FLY_SHORT_AUDIO_PATH;	// for purposes, I'm using the short audio
		break;
	case 3: // square block :o
		return "";
		break;
	case 4: // explosion1
		return C_BLAST_SM_AUDIO_PATH;	// not sure how big of an explosion this is
		break;
	case 5: // explosion2
		return C_BLAST_SM_AUDIO_PATH;	// this too
		break;
	case 6: // mosquito
		return C_MOSQUITO_AUDIO_PATH;
		break;
	case 7: // venus_fly_trap
		return ""; // C_VENUS_AUDIO_PATH;
		break;
	case 8: // massive_explosion
		return C_BLAST_LG_AUDIO_PATH;
		break;
	case 9: // large_explosion
		return C_BLAST_MD_AUDIO_PATH;
		break;
	default:
		return "";
	}
}

/*
*/
bool KamikazeFlyAudioHandler::isWithinRange(float x1, float y1, float x2, float y2)
{
	// Calculate the distance between the two
	float distance = sqrtf(pow(x1-x2, 2) + pow(y1-y2, 2));

	return (distance <= PLAYER_TO_BOT_RANGE);
}


bool KamikazeFlyAudioHandler::isBotInBotChannels(Bot *bot)
{
	map<int, Bot*>::iterator it;
	for (it = botChannels.begin(); it != botChannels.end(); it++)
	{
		Bot *b = it->second;
		if (b == bot)
			return true;
	}
	return false;
}

int KamikazeFlyAudioHandler::getChannelInBotChannels(Bot *bot)
{
	map<int, Bot*>::iterator it;
	for (it = botChannels.begin(); it != botChannels.end(); it++)
	{
		Bot *b = it->second;
		if (b == bot)
			return it->first;
	}
	return -1;
}

bool KamikazeFlyAudioHandler::botHasSFX(Bot *bot)
{
	int botId = bot->getSpriteType()->getSpriteTypeID();
	return (botId == 0 || botId == 1 || botId == 6);
}