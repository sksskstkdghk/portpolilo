#pragma once

struct ModelBone;

class ModelMesh
{
private:
	friend class Model;

	string name;

	int parentBoneIndex;
	ModelBone* parentBone;

	vector<ModelMeshPart*> meshParts;

	ModelMesh();
	~ModelMesh();

	void Binding();

public:
	void Render();

	int GetParentBoneIndex() { return parentBoneIndex; }
};