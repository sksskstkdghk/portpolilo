#include "Framework.h"

ModelClip::ModelClip(wstring file)
	: playTime(0.0f), isRepeat(false), speed(1.0f), isLockRoot(false), EndEvent(nullptr)
{
	ReadAnimation(file);
}

ModelClip::~ModelClip()
{
	for (auto key : keyFrameMap)
		delete key.second;
}

Matrix ModelClip::GetKeyFrameMatrix(ModelBone* bone)
{
	string boneName = bone->name;

	if (keyFrameMap.find(boneName) == keyFrameMap.end())
	{
		Matrix temp;
		D3DXMatrixIdentity(&temp);
		return temp;
	}

	ModelKeyFrame* keyFrame = keyFrameMap[boneName];

	playTime += speed * DELTA;

	if (isRepeat)
	{
		if (playTime >= duration)
		{
			while (playTime - duration >= 0)
				playTime -= duration;
		}
	}
	else
	{
		if (playTime >= duration)
		{
			playTime = duration;
			if (EndEvent != nullptr)
			{
				playTime = 0.0f;
				EndEvent();
			}
		}
	}

	Matrix invGlobal = bone->global;
	D3DXMatrixInverse(&invGlobal, nullptr, &invGlobal);

	Matrix animation = keyFrame->GetInterpolatedMatrix(playTime);

	Matrix parent;
	int parentIndex = bone->parentIndex;
	if (parentIndex < 0)
	{
		if (isLockRoot)
			D3DXMatrixIdentity(&parent);
		else
			parent = animation;
	}
	else
	{
		parent = animation * bone->parent->global;
	}

	return invGlobal * parent;
}

void ModelClip::UpdateKeyFrame(ModelBone* bone)
{
	bone->local = GetKeyFrameMatrix(bone);
}

void ModelClip::ReadAnimation(wstring file)
{
	BinaryReader* r = new BinaryReader(file);

	name = r->String();
	duration = r->Float();
	frameRate = r->Float();
	frameCount = r->Int();
	
	UINT count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelKeyFrame* key = new ModelKeyFrame();
		key->boneName = r->String();
		key->duration = duration;
		key->frameCount = frameCount;
		key->frameRate = frameRate;

		UINT size = r->UInt();
		if (size > 0)
		{
			key->transforms.resize(size);

			void* ptr = (void*)key->transforms.data();
			r->Byte(&ptr, sizeof(FbxKeyFrameData) * size);
		}
		keyFrameMap.insert({ key->boneName, key });
	}

	delete r;
}

void ModelClip::Reset()
{
	isRepeat = false;
	speed = 1.0f;
	playTime = 0.0f;
}
