#pragma once

class GameAnimModel : public GameModel
{
protected:
	vector<ModelClip*> clips;
	ModelTweener* tweener;

public:
	GameAnimModel(string file);
	virtual ~GameAnimModel();

	void Update();

	UINT AddClip(wstring file);
};