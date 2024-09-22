#include "Framework.h"

GameAnimModel::GameAnimModel(string file)
	: GameModel(file)
{
	model->SetShader(L"ModelAnimation");
	tweener = new ModelTweener();
}

GameAnimModel::~GameAnimModel()
{
	for (ModelClip* clip : clips)
		delete clip;

	delete tweener;
}

void GameAnimModel::Update()
{
	if (clips.empty())
		return;

	vector<ModelBone*>* bones = model->GetBones();

	for (UINT i = 0; i < bones->size(); i++)
	{
		ModelBone* bone = bones->at(i);
		tweener->UpdateBlending(bone);
	}

	GameModel::Update();
}

UINT GameAnimModel::AddClip(wstring file)
{
	file = L"FbxData/Clips/" + file;
	ModelClip* clip = new ModelClip(file);
	clips.push_back(clip);

	return clips.size() - 1;
}
