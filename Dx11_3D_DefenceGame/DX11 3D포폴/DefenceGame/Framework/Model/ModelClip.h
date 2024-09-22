#pragma once

class ModelClip
{
private:
	string name;

	float duration;
	float frameRate;
	int frameCount;

	bool isLockRoot;
	bool isRepeat;
	float speed;
	float playTime;

	unordered_map<string, ModelKeyFrame*> keyFrameMap;

	function<void()> EndEvent;
public:
	ModelClip(wstring file);
	~ModelClip();

	Matrix GetKeyFrameMatrix(ModelBone* bone);
	void UpdateKeyFrame(ModelBone* bone);

	void ReadAnimation(wstring file);

	void Reset();

	void LockRoot(bool val) { isLockRoot = val; }
	void Repeat(bool val) { isRepeat = val; }
	void Speed(float val) { speed = val; }
	void StartTime(float val) { playTime = val; }

	void SetEndEvent(function<void()> Event) { EndEvent = Event; }
};