#pragma once

class AModelClip
{
private:
	friend class AModel;

	string name;

	float duration;
	float frameRate;
	UINT frameCount;

	unordered_map<string, AssimpKeyFrame*> keyFrameMap;

	AModelClip();
	~AModelClip();

public:
	AssimpKeyFrame* KeyFrame(string name);

	float Duration() { return duration; }
	float FrameRate() { return frameRate; }
	UINT FrameCount() { return frameCount; }
};