#include "Framework.h"

AModel::AModel()
{
}

AModel::~AModel()
{
	for (Material* material : materials)
		delete material;

	for (AModelBone* bone : bones)
		delete bone;

	for (AModelMesh* mesh : meshes)
		delete mesh;
}

void AModel::ReadMaterial(string file, string folder)
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

		if (node->GetText())
		{
			string file = node->GetText();
			if (Utility::IsExistFile(folder + file))
			{
				wstring path = Utility::ToWString(folder + file);
				material->SetDiffuseMap(path);
			}
		}

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			string file = node->GetText();
			if (Utility::IsExistFile(folder + file))
			{
				wstring path = Utility::ToWString(folder + file);
				material->SetSpecularMap(path);
			}
		}

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			string file = node->GetText();
			if (Utility::IsExistFile(folder + file))
			{
				wstring path = Utility::ToWString(folder + file);
				material->SetNormalMap(path);
			}
		}

		Color color;

		node = node->NextSiblingElement();
		color.r = node->FloatAttribute("R");
		color.g = node->FloatAttribute("G");
		color.b = node->FloatAttribute("B");
		color.a = node->FloatAttribute("A");
		material->GetBuffer()->data.ambient = color;

		node = node->NextSiblingElement();
		color.r = node->FloatAttribute("R");
		color.g = node->FloatAttribute("G");
		color.b = node->FloatAttribute("B");
		color.a = node->FloatAttribute("A");
		material->GetBuffer()->data.diffuse = color;

		node = node->NextSiblingElement();
		color.r = node->FloatAttribute("R");
		color.g = node->FloatAttribute("G");
		color.b = node->FloatAttribute("B");
		color.a = node->FloatAttribute("A");
		material->GetBuffer()->data.specular = color;

		materials.push_back(material);

		matNode = matNode->NextSiblingElement();
	} while (matNode != nullptr);
}

void AModel::ReadMesh(string file, string folder)
{
	folder = "FbxData/Meshes/" + folder;
	BinaryReader* r = new BinaryReader(Utility::ToWString(folder + file));

	UINT count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		AModelBone* bone = new AModelBone();

		bone->index = r->Int();
		bone->name = r->String();
		bone->parentIndex = r->Int();
		bone->transform = r->Matrix();		

		bones.push_back(bone);
	}

	count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		AModelMesh* mesh = new AModelMesh();
		mesh->name = r->String();
		mesh->boneIndex = r->Int();

		mesh->materialName = r->String();

		{//VertexData
			UINT count = r->UInt();

			mesh->vertexCount = count;
			mesh->vertices = new ModelVertexType[count];

			void* ptr = (void*)mesh->vertices;
			r->Byte(&ptr, sizeof(ModelVertexType) * count);			
		}

		{//IndexData
			UINT count = r->UInt();

			mesh->indexCount = count;
			mesh->indices = new UINT[count];

			void* ptr = (void*)mesh->indices;
			r->Byte(&ptr, sizeof(UINT) * count);
		}

		meshes.push_back(mesh);
	}

	delete r;

	BindingBone();
	BindingMesh();
}

void AModel::ReadClip(string file, string folder)
{
	folder = "FbxData/Clips/" + folder;
	BinaryReader* r = new BinaryReader(Utility::ToWString(folder + file));

	AModelClip* clip = new AModelClip();

	clip->name = r->String();
	clip->duration = r->Float();
	clip->frameRate = r->Float();
	clip->frameCount = r->UInt();

	UINT keyFrameCount = r->UInt();
	for (UINT i = 0; i < keyFrameCount; i++)
	{
		AssimpKeyFrame* keyFrame = new AssimpKeyFrame();
		keyFrame->boneName = r->String();

		UINT size = r->UInt();
		if (size > 0)
		{
			keyFrame->transforms.resize(size);

			void* ptr = (void*)keyFrame->transforms.data();
			r->Byte(&ptr, sizeof(AssimpKeyFrameData) * size);
		}
		clip->keyFrameMap[keyFrame->boneName] = keyFrame;
	}
	
	clips.push_back(clip);

	delete r;
}

void AModel::BindingBone()
{
	root = bones[0];
	for (AModelBone* bone : bones)
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

void AModel::BindingMesh()
{
	for (AModelMesh* mesh : meshes)
	{
		for (AModelBone* bone : bones)
		{
			if (mesh->boneIndex == bone->index)
			{
				mesh->bone = bone;
				break;
			}
		}

		mesh->Binding(this);
	}
}

void AModel::SetDiffuseMap(wstring file)
{
}

void AModel::SetSpecularMap(wstring file)
{
}

void AModel::SetNormalMap(wstring file)
{
}

AModelBone* AModel::BoneByName(string name)
{
	for (AModelBone* bone : bones)
	{
		if (bone->name == name)
			return bone;
	}

	return nullptr;
}

Material* AModel::MaterialByName(string name)
{
	for (Material* material : materials)
	{
		if (material->Name() == name)
			return material;
	}

	return nullptr;
}

AModelClip* AModel::ClipByName(string name)
{
	for (AModelClip* clip : clips)
		if (clip->name == name)
			return clip;

	return nullptr;
}
