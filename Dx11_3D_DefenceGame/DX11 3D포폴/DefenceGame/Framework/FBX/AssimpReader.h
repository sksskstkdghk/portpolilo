#pragma once

class AssimpReader
{
private:
	Assimp::Importer* importer;
	const aiScene* scene;
	
	vector<AssimpMaterial*> materials;
	vector<AssimpBone*> bones;
	vector<AssimpMesh*> meshes;

public:
	AssimpReader(string file);
	~AssimpReader();

	void ExporterMaterial(string saveFile, bool isOverwrite = true);
	void ExporterMesh(string saveFile, bool isOverwrite = true);
	void ExporterAnimClip(UINT index, string saveFile, bool isOverwrite = true);

	vector<string> ClipList();
private:
	void ReadMaterial();
	void WriteMaterial(string saveFile, bool isOverwrite);	
	string WriteTexture(string savePath, string file);

	void ReadBone(aiNode* node, int index, int parent);
	void ReadMesh(aiNode* node, int bone);	
	void ReadSkin();
	void WriteMesh(string saveFile, bool isOverwrite);

	AssimpClip* ReadClipData(aiAnimation* animation);
	void ReadKeyframeData(AssimpClip* clip, aiNode* node, vector<AssimpClipNode>& nodeInfos);
	void WriteClipData(AssimpClip* clip, string saveFile, bool isOverwrite);
};