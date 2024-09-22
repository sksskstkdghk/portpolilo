#include "Framework.h"

ModelTweener::ModelTweener()
	: current(nullptr), next(nullptr), blendTime(0.0f), elapsedTime(0.0f)
{
}

ModelTweener::~ModelTweener()
{
}

void ModelTweener::Play(ModelClip* clip, bool isRepeat, float blendTime, float speed, float startTime)
{
	this->blendTime = blendTime;
	this->elapsedTime = startTime;

	if (current != nullptr)
	{
		next = clip;
		next->Repeat(isRepeat);
		next->StartTime(startTime);
		next->Speed(speed);
	}
	else
	{
		current = clip;
		current->Repeat(isRepeat);
		current->StartTime(startTime);
		current->Speed(speed);
	}
}

void ModelTweener::UpdateBlending(ModelBone* bone)
{
	Matrix matrix;
	if (next != nullptr)
	{
		elapsedTime += DELTA;
		float t = elapsedTime / blendTime;

		if (t > 1.0f)
		{
			matrix = next->GetKeyFrameMatrix(bone);

			current = next;
			next = nullptr;
		}
		else
		{
			Matrix start = current->GetKeyFrameMatrix(bone);
			Matrix end = next->GetKeyFrameMatrix(bone);

			//matrix = start * (1 - t) + end * t;
			matrix = LERP(start, end, t);
		}
	}
	else
	{
		matrix = current->GetKeyFrameMatrix(bone);
	}

	bone->local = matrix;
}
