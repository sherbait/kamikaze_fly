#include "stdafx.h"
#include "AudioManager.h"
#include "fmod.hpp"
#include "fmod_errors.h"

void AudioManager::errorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cerr << "FMOD error - " << result << " - " << FMOD_ErrorString(result);
	}
}

bool AudioManager::init()
{
	int numDrivers;
	char name[256];
	//float DISTANCE_FACTOR = 100;

	result = FMOD::System_Create(&system);
	errorCheck(result);
	result = system->getVersion(&version);
	errorCheck(result);

    if (version < FMOD_VERSION)
    {
        std::cerr << "FMOD lib version %08x doesn't match header version %08x";
    }

	result = system->getNumDrivers(&numDrivers);
	errorCheck(result);

	if (numDrivers == 0)
	{		
        result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        errorCheck(result);    
	}
	else
    {
        result = system->getDriverCaps(0, &caps, 0, &speakermode);
        errorCheck(result);

        result = system->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
		errorCheck(result);

        if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
        {                                                   /* You might want to warn the user about this. */
            result = system->setDSPBufferSize(1024, 10);
            errorCheck(result);
        }

        result = system->getDriverInfo(0, name, 256, 0);
        errorCheck(result);

        if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
        {
            result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, FMOD_DSP_RESAMPLER_LINEAR);
            errorCheck(result);
        }
    }

	result = system->init(100, FMOD_INIT_NORMAL, 0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)          /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
    {
        result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        
		errorCheck(result);
            
        result = system->init(100, FMOD_INIT_NORMAL, 0); /* ... and re-init. */
        errorCheck(result);
    }

	//create2DSoundSample("data/audio/testSong.wav", &backgroundSound, true);
	//playSound(backgroundSound, &backgroundChannel);	

	result = system->set3DSettings(1.0f, DISTANCE_FACTOR, 1.0f);
	errorCheck(result);

	size = 0;	/* There are currently no audio files*/

	return true;
}

bool AudioManager::isInRange(int i)
{
	return (i >= 0 && i < MAX_AUDIO_FILES);
}

void AudioManager::create2DSoundSample(const char* filePath, Sound** sound, bool loop)
{
	result = system->createSound(filePath, FMOD_2D, 0, sound);
    errorCheck(result);
	if (loop)
	{
		result = (*sound)->setMode(FMOD_LOOP_NORMAL);
	}
    errorCheck(result);

}

/*
	Basic function that plays the given sound in the given channel.
	The sound's properties are determined on how it was added using addSound().
*/
void AudioManager::playSound(Sound* sound, Channel** channel)
{
	result = system->playSound(sound, 0, false, channel);
    errorCheck(result);
}

/*
	DELETE THIS. This function must be called each time an entity moves relative to the listener's position.
	The listener's default position is {0.0, 0.0, 0.0} which is unchangeable in this implementation.
*/
void AudioManager::playSoundAtPosition(Sound *sound, Channel **channel, float pX, float pY, float vX, float vY)
{
	// This is for an entity other than the listener
	//FMOD_VECTOR position = {pX * DISTANCE_FACTOR, pY * DISTANCE_FACTOR, 0.0f};
	//FMOD_VECTOR velocity = {vX * DISTANCE_FACTOR, vY * DISTANCE_FACTOR, 0.0f};

	//result = system->playSound(sound, 0, true, channel);
 //   errorCheck(result);
	//result = (*channel)->set3DAttributes(&position, &velocity);
	//errorCheck(result);
	//result = (*channel)->setPaused(false);
	//errorCheck(result);

	//// Listener attributes
	//FMOD_VECTOR pos = {0.0, 0.0, 0.0};
	//FMOD_VECTOR vel = {0.0, 0.0, 0.0};
	//FMOD_VECTOR forward = {0.0, 0.0, 1.0};	// fmod uses LHS coordinate system by default
	//FMOD_VECTOR up = {0.0, 1.0, 0.0};
	//result = system->get3DListenerAttributes(0, &pos, &vel, &forward, &up);
	//errorCheck(result);
}

/*
	Creates a 2D Sound and stores it.
*/
void AudioManager::addSound(const char* filePath, bool loop)
{
	if (size < MAX_AUDIO_FILES)
	{
		// Create the sound
		result = system->createSound(filePath, FMOD_2D, 0, &(sounds[size].sound));
		errorCheck(result);
		
		// Store the filename
		sounds[size].filePath = filePath;

		// Will this sound loop?
		if (loop)
		{
			result = sounds[size].sound->setMode(FMOD_LOOP_NORMAL);
			errorCheck(result);
		}

		// Increment the audio count
		size++;
	}
}

/*
	Creates a 3D sound and stores it.

	min/max determines how loud the sound is playing relative to the listener position.
*/
void AudioManager::addSound(const char* filePath, bool loop, int min, int max)
{
	/*if (size < MAX_AUDIO_FILES)
	{
		// Create the sound
		result = system->createSound(filePath, FMOD_3D, 0, &(sounds[size].sound));
		errorCheck(result);
		
		// Store the filename
		sounds[size].filePath = filePath;

		// Will this sound loop?
		if (loop)
		{
			result = sounds[size].sound->setMode(FMOD_LOOP_NORMAL);
			errorCheck(result);
		}

		// This is a 3D sound so we need to know when or when not to play the sound
		result = sounds[size].sound->set3DMinMaxDistance(min * DISTANCE_FACTOR, max * DISTANCE_FACTOR);
		errorCheck(result);

		// Increment the audio count
		size++;
	}*/
}

Sound* AudioManager::getSound(const char* filePath)
{
	for (int i = 0; i < size; i++)
	{
		if (strcmp(sounds[i].filePath, filePath) == 0)
			return sounds[i].sound;
	}
	return NULL;
}

Channel** AudioManager::getChannel(int i)
{
	if (isInRange(i))
	{
		return &channels[i];
	}
	return NULL;
}

bool AudioManager::isChannelPlaying(int i)
{
	if (isInRange(i))
	{
		bool isPlaying = 0;
		channels[i]->isPlaying(&isPlaying);
		return isPlaying;
	}
}

/*
	Stops all Sounds that are currently in Channel i.
*/
void AudioManager::stopChannel(int i)
{
	if (isInRange(i))
	{
		channels[i]->stop();
	}
}

/*
	Pause/Unpause the channel at index i.
*/
void AudioManager::toggleChannel(int i)
{
	if (isInRange(i))
	{
		bool paused = 0;
		channels[i]->getPaused(&paused);
		channels[i]->setPaused(!paused);
	}
}

void AudioManager::setChannelVolume(int channelIndex, float volume)
{
	if (isInRange(channelIndex))
	{
		channels[channelIndex]->setVolume(volume);
	}
}

/*
	This must be called once per frame.
*/
void AudioManager::update()
{
	result = system->update();
	errorCheck(result);
}

/*
	Releases all members.
*/
void AudioManager::shutdown()
{
	for (int i = 0; i < size; i++)
	{
		sounds[i].sound->release();
	}

	result = system->close();
	errorCheck(result);
    result = system->release();
	errorCheck(result);
}