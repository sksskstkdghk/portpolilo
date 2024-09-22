#pragma once

struct AModelBone
{
	int index;
	string name;
	int parentIndex;
	AModelBone* parent;

	vector<AModelBone*> children;

	Matrix transform;
};

class AModel
{
private:
	AModelBone* root;

	vector<Material*> materials;
	vector<AModelBone*> bones;
	vector<AModelMesh*> meshes;
	vector<AModelClip*> clips;

public:
	AModel();
	~AModel();

	void ReadMaterial(string file, string folder = "");
	void ReadMesh(string file, string folder = "");
	void ReadClip(string file, string folder = "");

	void BindingBone();
	void BindingMesh();
		
	void SetDiffuseMap(wstring file);
	void SetSpecularMap(wstring file);
	void SetNormalMap(wstring file);

	vector<AModelBone*>* GetBones() { return &bones; }
	vector<AModelMesh*>* GetMeshes() { return &meshes; }
	vector<AModelClip*>* GetClips() { return &clips; }

	AModelBone* BoneByName(string name);
	Material* MaterialByName(string name);
	AModelClip* ClipByIndex(UINT index) { return clips[index]; }
	AModelClip* ClipByName(string name);

};