#include "Framework.h"
#include "FmodSound.h"

FmodSound* FmodSound::instance = nullptr;

FmodSound::~FmodSound()
{
	sound->release();

	soundSystem->close();
	soundSystem->release();
}

void FmodSound::Init()
{
	result = System_Create(&soundSystem);
	assert(result == FMOD_OK);

	result = soundSystem->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	channel = nullptr;
}

void FmodSound::Update()
{
	soundSystem->update();
}

void FmodSound::PlaySound(string soundName, UINT loop)
{
	bool temp;
	channel->isPlaying(&temp);

	if (temp)
		channel->stop();

	if (soundName != "")
		result = soundSystem->createSound(soundName.c_str(), loop, 0, &sound);
	assert(result == FMOD_OK);

	result = soundSystem->playSound(sound, 0, false, &channel);
	assert(result == FMOD_OK);
}

void FmodSound::SetVolume(float volume)
{
	if(volume > SOUND_MAX)
		result = channel->setVolume(SOUND_MAX);
	else if(volume < SOUND_MIN)
		result = channel->setVolume(SOUND_MIN);
	else
		result = channel->setVolume(volume);

	assert(result == FMOD_OK);
}
