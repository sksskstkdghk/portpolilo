#pragma once

class GameModel : public Transform
{
protected:
	Model* model;
	IntBuffer* intBuffer;
	vector<Matrix> boneTransforms;

public:
	GameModel(string file);
	GameModel(string file, wstring shaderName);
	~GameModel();

	void Update();
	void Render();

	Model* GetModel() { return model; }
};