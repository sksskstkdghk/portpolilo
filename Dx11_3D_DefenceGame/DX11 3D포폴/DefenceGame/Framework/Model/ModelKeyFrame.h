#pragma once

class ModelKeyFrame
{
private:
	friend class ModelClip;

	string boneName;

	float duration;
	float frameRate;
	int frameCount;

	vector<FbxKeyFrameData> transforms;

public:
	ModelKeyFrame();
	~ModelKeyFrame();

	Matrix GetInterpolatedMatrix(float time);
	Matrix GetInterpolatedMatrix(UINT index1, UINT index2, float t);

	UINT GetKeyFrameIndex(float time);
	void CalcKeyFrameIndex(float time, OUT UINT& index1, OUT UINT& index2,
		OUT float& interpolatedTime);
};