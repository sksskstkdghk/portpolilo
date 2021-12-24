#include "Framework.h"

Model::Model()
{
	boneBuffer = new BoneBuffer();
}

Model::~Model()
{
	delete boneBuffer;

	for (Material* material : materials)
		delete material;

	for (ModelBone* bone : bones)
		delete bone;

	for (ModelMesh* mesh : meshes)
		delete mesh;
}

void Model::ReadMaterial(string file, string folder)
{
	folder = "FbxData/Materials/" + folder;

	XmlDocument* document = new XmlDocument();
	document->LoadFile((folder + file).c_str());

	XmlElement* root = document->FirstChildElement();
	XmlElement* matNode = root->FirstChildElement();

	do
	{
		XmlElement* node = matNode->FirstChildElement();

		Material* material = new Material();
		material->Name(node->GetText());

		node = node->NextSiblingElement();

		XmlElement* color = node->FirstChildElement();
		Color diffuse;
		diffuse.r = color->FloatText();

		color = color->NextSiblingElement();
		diffuse.g = color->FloatText();

		color = color->NextSiblingElement();
		diffuse.b = color->FloatText();

		color = color->NextSiblingElement();
		diffuse.a = color->FloatText();

		material->GetBuffer()->data.diffuse = diffuse;

		node = node->NextSiblingElement();		

		if (node->GetText())
		{
			string diffuseFile = node->GetText();
			wstring path = Utility::ToWString(folder + diffuseFile);
			material->SetDiffuseMap(path);
		}else
			material->SetDiffuseMap(L"FbxData/Materials/DefaultTex.png");

		materials.push_back(material);

		matNode = matNode->NextSiblingElement();
	} while (matNode != nullptr);
}

void Model::ReadMesh(string file, string folder)
{
	folder = "FbxData/Meshes/" + folder;
	BinaryReader* r = new BinaryReader(Utility::ToWString(folder + file));

	UINT count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		ModelBone* bone = new ModelBone();

		bone->index = r->Int();
		bone->name = r->String();
		bone->parentIndex = r->Int();

		bone->local = r->Matrix();
		bone->global = r->Matrix();

		bones.push_back(bone);
	}

	count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();
		mesh->name = r->String();
		mesh->parentBoneIndex = r->Int();

		UINT partCount = r->UInt();
		for (UINT j = 0; j < partCount; j++)
		{
			ModelMeshPart* meshPart = new ModelMeshPart();
			meshPart->parent = mesh;
			meshPart->materialName = r->String();

			{
				UINT count = r->UInt();
				meshPart->vertices.resize(count);

				void* ptr = (void*)meshPart->vertices.data();
				r->Byte(&ptr, sizeof(ModelVertexType) * count);
			}

			{
				UINT count = r->UInt();
				meshPart->indices.resize(count);

				void* ptr = (void*)meshPart->indices.data();
				r->Byte(&ptr, sizeof(UINT) * count);
			}

			mesh->meshParts.push_back(meshPart);
		}
		meshes.push_back(mesh);
	}

	delete r;

	BindingBone();
	BindingMesh();
}

void Model::CopyGlobalBoneTo(vector<Matrix>& transforms)
{
	Matrix w;
	D3DXMatrixIdentity(&w);

	CopyGlobalBoneTo(transforms, w);
}

void Model::CopyGlobalBoneTo(vector<Matrix>& transforms, Matrix& w)
{
	transforms.clear();
	transforms.resize(bones.size());

	for (UINT i = 0; i < bones.size(); i++)
	{
		ModelBone* bone = bones[i];

		if (bone->parent != nullptr)
		{
			int index = bone->parent->index;
			transforms[i] = bone->local * transforms[index];
		}
		else
			transforms[i] = bone->local * w;
	}
}

void Model::BindingBone()
{
	root = bones.front();

	for (ModelBone* bone : bones)
	{
		if (bone->parentIndex > -1)
		{
			bone->parent = bones[bone->parentIndex];
			bone->parent->children.push_back(bone);
		}
		else
			bone->parent = nullptr;
	}
}

void Model::BindingMesh()
{
	for (ModelMesh* mesh : meshes)
	{
		for (ModelBone* bone : bones)
		{
			if (mesh->parentBoneIndex == bone->index)
			{
				mesh->parentBone = bone;
				break;
			}
		}

		for (ModelMeshPart* part : mesh->meshParts)
		{
			for (Material* material : materials)
			{
				if (material->Name() == part->materialName)
				{
					part->material = material;
					break;
				}
			}
		}

		mesh->Binding();
	}
}

void Model::SetShader(wstring file)
{
	Shader* shader = Shader::Add(file);

	for (Material* material : materials)
		material->SetShader(shader);
}

void Model::SetBones(Matrix* m, UINT count)
{
	boneBuffer->Bones(m, count);
	boneBuffer->SetVSBuffer(2);
}

void Model::SetDiffuseMap(wstring file)
{
	for (Material* material : materials)
		material->SetDiffuseMap(file);
}

void Model::SetSpecularMap(wstring file)
{
	for (Material* material : materials)
		material->SetSpecularMap(file);
}

void Model::SetNormalMap(wstring file)
{
	for (Material* material : materials)
		material->SetNormalMap(file);
}

ModelBone* Model::BoneByName(string name)
{
	for (ModelBone* bone : bones)
	{
		if (bone->name == name)
			return bone;
	}

	return nullptr;
}
