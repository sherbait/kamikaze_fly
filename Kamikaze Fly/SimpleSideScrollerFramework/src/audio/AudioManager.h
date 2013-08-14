#pragma once
#include "fmod.hpp"
#include "stdafx.h"
#include "AudioHandler.h"

class Game;

using namespace FMOD;
using namespace std;

const static unsigned int MAX_AUDIO_FILES = 25;
const float DISTANCE_FACTOR = 1.0f;

struct track {
	const char		*filePath;
	FMOD::Sound		*sound;
};

class AudioManager
{
protected:
	AudioHandler *audioHandler;
private:
	FMOD::System		*system;
	track				sounds[MAX_AUDIO_FILES];
    FMOD::Channel		*channels[MAX_AUDIO_FILES];
    FMOD_RESULT			result;
	FMOD_SPEAKERMODE	speakermode;
	FMOD_CAPS			caps;
    unsigned int		version;
	int					size;

	//Set these private for the singleton design pattern
	AudioManager()
	{	
		system = 0; 
		for (int i = 0; i < MAX_AUDIO_FILES; i++)
			channels[i] = 0;
		init();
	}
	~AudioManager()		{}
	AudioManager(AudioManager const&){}
	AudioManager& operator=(AudioManager const&){}

	void errorCheck(FMOD_RESULT);
	bool init();
	bool isInRange(int i);
public:
	/*static AudioManager& getAudioManager()
	{
		static AudioManager instance;
		return instance;
	}*/
	static AudioManager* getAudioManager()
	{
		static AudioManager *instance;
		if (!instance)
		{
			instance = new AudioManager();
		}
		return instance;
	}
	
	void addSound(const char* filePath, bool loop);						/* For 2D sounds */
	void addSound(const char* filePath, bool loop, int min, int max);	/* For 3D sounds*/
	void create2DSoundSample(const char* filePath, Sound** sound, bool loop);
	bool isChannelPlaying(int i);
	void playSound(Sound* sound, Channel** channel);
	void playSoundAtPosition(Sound *sound, Channel **channel, float pX, float pY, float vX, float vY);
	void setChannelVolume(int channelIndex, float volume);
	void shutdown();
	void stopChannel(int i);
	void toggleChannel(int i);
	void update();

	/** INLINE METHODS **/
	void registerAudioHandler(AudioHandler *ah)		{	audioHandler = ah;						}
	void respondToGameEvents(Game *game)			{	audioHandler->handleGameEvents(game);	}
	void respondToButtonPress(Game *game)			{   audioHandler->handleButtonEvents(game);	}

	/** ACCESSOR METHODS **/
	Sound* getSound(const char* filePath);
	Channel** getChannel(int i);
};