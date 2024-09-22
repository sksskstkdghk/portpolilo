#include "Framework.h"

GameModel::GameModel(string file)
{
	model = new Model();
	model->ReadMaterial(file + ".mat", file + "/");
	model->ReadMesh(file + ".mesh", file + "/");

	model->SetShader(L"Model");

	intBuffer = new IntBuffer();
}

GameModel::GameModel(string file, wstring shaderName)
{
	model = new Model();
	model->ReadMaterial(file + ".mat", file + "/");
	model->ReadMesh(file + ".mesh", file + "/");

	model->SetShader(shaderName);

	intBuffer = new IntBuffer();
}

GameModel::~GameModel()
{
	delete model;
	delete intBuffer;
}

void GameModel::Update()
{
	model->CopyGlobalBoneTo(boneTransforms, world);

	UpdateWorld();
}

void GameModel::Render()
{
	model->SetBones(boneTransforms.data(), boneTransforms.size());

	vector<ModelMesh*>* meshes = model->GetMeshes();

	for (ModelMesh* mesh : *meshes)
	{
		int index = mesh->GetParentBoneIndex();

		intBuffer->data.index = index;
		intBuffer->SetVSBuffer(3);

		mesh->Render();
	}
}
