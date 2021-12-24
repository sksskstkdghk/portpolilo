#pragma once
#ifndef _SOUND_H_
#define _SOUND_H_

#include <fmod.hpp>

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

using namespace FMOD;

class FmodSound
{
private:
	System*	 soundSystem;
	Sound*	 sound;
	Channel* channel;

	FMOD_RESULT result;

	static FmodSound* instance;

	~FmodSound();

public:
	static FmodSound* Get() { return instance; }
	static void Create() { instance = new FmodSound(); }
	static void Delete() { delete instance; }

	void Init();
	void Update();

	void PlaySound(string soundName = "", UINT loop = FMOD_LOOP_OFF);

	void SetVolume(float volume);
};

#endif // !_SOUND_H_