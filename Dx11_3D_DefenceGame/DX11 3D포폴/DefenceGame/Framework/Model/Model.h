#pragma once

struct ModelBone
{
	int index;
	string name;
	int parentIndex;
	ModelBone* parent;

	vector<ModelBone*> children;

	Matrix local;
	Matrix global;
};

class Model
{
private:
	BoneBuffer* boneBuffer;

	ModelBone* root;

	vector<Material*> materials;
	vector<ModelBone*> bones;
	vector<ModelMesh*> meshes;

public:
	Model();
	~Model();

	void ReadMaterial(string file, string folder = "");
	void ReadMesh(string file, string folder = "");

	void CopyGlobalBoneTo(vector<Matrix>& transforms);
	void CopyGlobalBoneTo(vector<Matrix>& transforms, Matrix& w);

	void BindingBone();
	void BindingMesh();

	void SetShader(wstring file);
	void SetBones(Matrix* m, UINT count);

	void SetDiffuseMap(wstring file);
	void SetSpecularMap(wstring file);
	void SetNormalMap(wstring file);

	vector<ModelBone*>* GetBones() { return &bones; }
	vector<ModelMesh*>* GetMeshes() { return &meshes; }
	ModelBone* BoneByName(string name);
};