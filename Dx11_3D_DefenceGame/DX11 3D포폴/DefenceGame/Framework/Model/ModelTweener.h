#pragma once

class ModelTweener
{
private:
	ModelClip* current;
	ModelClip* next;

	float blendTime;
	float elapsedTime;

public:
	ModelTweener();
	~ModelTweener();

	void Play(ModelClip* clip, bool isRepeat = false, float blendTime = 10.0f,
		float speed = 15.0f, float startTime = 0.0f);

	void UpdateBlending(ModelBone* bone);
};