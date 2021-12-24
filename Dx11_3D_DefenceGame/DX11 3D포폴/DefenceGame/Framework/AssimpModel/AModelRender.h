#pragma once

class AModelRender
{
private:
	AModel* model;

	Matrix bones[MAX_MODEL_BONE];
	Matrix boneTransforms[MAX_MODEL_INSTANCE][MAX_MODEL_BONE];

	vector<Transform*> transforms;
	Matrix worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

public:
	AModelRender(Shader* shader, string name);
	~AModelRender();

	void Update();
	void Render();

	void UpdateTransform(UINT instanceID, UINT boneIndex, Transform& transform);
	void UpdateTransforms();

	Transform* AddTransform();

	AModel* GetModel() { return model; }

private:
	void CreateTexture();
};