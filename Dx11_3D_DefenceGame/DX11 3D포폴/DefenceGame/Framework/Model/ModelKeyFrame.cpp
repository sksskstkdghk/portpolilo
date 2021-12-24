#include "Framework.h"

ModelKeyFrame::ModelKeyFrame()
{
}

ModelKeyFrame::~ModelKeyFrame()
{
}

Matrix ModelKeyFrame::GetInterpolatedMatrix(float time)
{
	UINT index1 = 0;
	UINT index2 = 0;
	float interpolatedTime = 0.0f;

	CalcKeyFrameIndex(time, index1, index2, interpolatedTime);

	return GetInterpolatedMatrix(index1, index2, interpolatedTime);
}

Matrix ModelKeyFrame::GetInterpolatedMatrix(UINT index1, UINT index2, float t)
{
	Vector3 scale;
	D3DXVec3Lerp(&scale, &transforms[index1].scale, &transforms[index2].scale, t);

	Quaternion rotation;
	D3DXQuaternionSlerp(&rotation, &transforms[index1].rotation, &transforms[index2].rotation, t);

	Vector3 position;
	D3DXVec3Lerp(&position, &transforms[index1].position, &transforms[index2].position, t);

	Matrix S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationQuaternion(&R, &rotation);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	return S * R * T;
}

UINT ModelKeyFrame::GetKeyFrameIndex(float time)
{
	UINT start = 0, end = frameCount - 1;
	if (time >= transforms[end].time)
		return end;

	do 
	{
		UINT middle = (start + end) / 2;

		if (end - start <= 1)
			break;
		else if (transforms[middle].time < time)
			start = middle;
		else if (transforms[middle].time > time)
			end = middle;
		else
		{
			start = middle;
			break;
		}
	}while ((end - start) > 1);

	return start;
}

void ModelKeyFrame::CalcKeyFrameIndex(float time, OUT UINT& index1, OUT UINT& index2, OUT float& interpolatedTime)
{
	index1 = GetKeyFrameIndex(time);
	index2 = index1 + 1;

	if (index1 >= frameCount - 1)
	{
		index1 = index2 = frameCount - 1;
		interpolatedTime = 1.0f;
	}
	else
	{
		float time1 = time - transforms[index1].time;
		float time2 = transforms[index2].time - transforms[index1].time;

		interpolatedTime = time1 / time2;
	}
}
