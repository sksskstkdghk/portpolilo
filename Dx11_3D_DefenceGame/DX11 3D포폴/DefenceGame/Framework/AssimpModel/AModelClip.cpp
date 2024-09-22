#include "Framework.h"

AModelClip::AModelClip()
{
}

AModelClip::~AModelClip()
{
}

AssimpKeyFrame* AModelClip::KeyFrame(string name)
{
	if (keyFrameMap.count(name) == 0)
		return nullptr;

	return keyFrameMap[name];
}
