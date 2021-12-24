#include "Framework.h"

AssimpReader::AssimpReader(string file)
{
	importer = new Assimp::Importer();

	scene = importer->ReadFile(file,
		aiProcess_ConvertToLeftHanded | 
		aiProcessPreset_TargetRealtime_MaxQuality);
	assert(scene != nullptr);
}

AssimpReader::~AssimpReader()
{
	delete importer;
}

void AssimpReader::ExporterMaterial(string saveFile, bool isOverwrite)
{
	ReadMaterial();
	saveFile = "FbxData/Materials/" + saveFile + ".mat";
	WriteMaterial(saveFile, isOverwrite);
}

void AssimpReader::ExporterMesh(string saveFile, bool isOverwrite)
{
	ReadBone(scene->mRootNode, -1, -1);	
	ReadSkin();
	saveFile = "FbxData/Meshes/" + saveFile + ".mesh";
	WriteMesh(saveFile, isOverwrite);
}

void AssimpReader::ExporterAnimClip(UINT index, string saveFile, bool isOverwrite)
{
	AssimpClip* clip = ReadClipData(scene->mAnimations[index]);
	saveFile = "FbxData/Clips/" + saveFile + ".clip";
	WriteClipData(clip, saveFile, isOverwrite);
}

vector<string> AssimpReader::ClipList()
{
	vector<string> result;
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* anim = scene->mAnimations[i];
		result.push_back(anim->mName.C_Str());
	}

	return result;
}

void AssimpReader::ReadMaterial()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];

		AssimpMaterial* material = new AssimpMaterial();

		material->name = srcMaterial->GetName().C_Str();

		aiColor3D color;
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambient = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.a);

		aiString file;
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->diffuseFile = file.C_Str();
		
		file.Clear();
		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->specularFile = file.C_Str();

		file.Clear();
		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->normalFile = file.C_Str();

		materials.push_back(material);
	}
}

void AssimpReader::WriteMaterial(string saveFile, bool isOverwrite)
{
	if (!isOverwrite)
	{
		if (Utility::IsExistFile(saveFile))
			return;
	}

	Utility::CreateFolders(saveFile, true);

	XmlDocument* document = new XmlDocument();
	XmlElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);

	string folder = Utility::GetDirectoryName(saveFile);

	for (AssimpMaterial* material : materials)
	{
		XmlElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		XmlElement* element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("DiffuseFile");
		string temp = WriteTexture(folder, material->diffuseFile);
		element->SetText(temp.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		temp = WriteTexture(folder, material->specularFile);
		element->SetText(temp.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		temp = WriteTexture(folder, material->normalFile);
		element->SetText(temp.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->ambient.r);
		element->SetAttribute("G", material->ambient.g);
		element->SetAttribute("B", material->ambient.b);
		element->SetAttribute("A", material->ambient.a);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->diffuse.r);
		element->SetAttribute("G", material->diffuse.g);
		element->SetAttribute("B", material->diffuse.b);
		element->SetAttribute("A", material->diffuse.a);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->specular.r);
		element->SetAttribute("G", material->specular.g);
		element->SetAttribute("B", material->specular.b);
		element->SetAttribute("A", material->specular.a);
		node->LinkEndChild(element);

		delete material;
	}

	document->SaveFile(saveFile.c_str());

	delete document;
}

string AssimpReader::WriteTexture(string savePath, string file)
{
	if (file.length() == 0)
		return "";

	string fileName = Utility::GetFileName(file);
	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

	if (texture)
	{
		if (texture->mHeight)
		{
			ID3D11Texture2D* dest;
			D3D11_TEXTURE2D_DESC destDesc = {};
			destDesc.Width = texture->mWidth;
			destDesc.Height = texture->mHeight;
			destDesc.MipLevels = 1;
			destDesc.ArraySize = 1;
			destDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			destDesc.SampleDesc.Count = 1;
			destDesc.SampleDesc.Quality = 0;
			destDesc.Usage = D3D11_USAGE_IMMUTABLE;

			D3D11_SUBRESOURCE_DATA subResource = {};
			subResource.pSysMem = texture->pcData;

			DEVICE->CreateTexture2D(&destDesc, &subResource, &dest);

			D3DX11SaveTextureToFileA(DC, dest, D3DX11_IFF_PNG, savePath.c_str());
		}
		else
		{
			BinaryWriter w(Utility::ToWString(savePath + fileName));
			w.Byte(texture->pcData, texture->mWidth);
		}
	}

	return fileName;
}

void AssimpReader::ReadBone(aiNode* node, int index, int parent)
{
	AssimpBone* bone = new AssimpBone();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	Matrix transform(node->mTransformation[0]);
	D3DXMatrixTranspose(&bone->transform, &transform);

	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->transform;

	bone->transform = bone->transform * matParent;
	bones.push_back(bone);

	ReadMesh(node, index);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBone(node->mChildren[i], bones.size(), index);
}

void AssimpReader::ReadMesh(aiNode* node, int bone)
{
	if (node->mNumMeshes == 0)
		return;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		AssimpMesh* mesh = new AssimpMesh();
		mesh->name = node->mName.C_Str();
		mesh->boneIndex = bone;

		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = material->GetName().C_Str();

		UINT startVertex = mesh->vertices.size();
		UINT startIndex = mesh->indices.size();

		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			ModelVertexType vertex;
			memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Vector3));
			memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));
			memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Vector3));
			if (srcMesh->HasTangentsAndBitangents())
			{
				memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Vector3));
			}
			mesh->vertices.push_back(vertex);
		}

		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				mesh->indices.push_back(face.mIndices[k]);
				mesh->indices.back() += startVertex;
			}
		}

		meshes.push_back(mesh);
	}
}

void AssimpReader::ReadSkin()
{
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene->mMeshes[i];
		if (!aiMesh->HasBones())
			continue;

		AssimpMesh* mesh = meshes[i];

		vector<AssimpBoneWeights> boneWeights;
		boneWeights.resize(mesh->vertices.size());

		for (UINT b = 0; b < aiMesh->mNumBones; b++)
		{
			aiBone* aiBone = aiMesh->mBones[b];

			UINT boneIndex = 0;
			for (AssimpBone* bone : bones)
			{
				if (bone->name == (string)aiBone->mName.C_Str())
				{
					boneIndex = bone->index;
					break;
				}
			}

			for (UINT w = 0; w < aiBone->mNumWeights; w++)
			{
				UINT index = aiBone->mWeights[w].mVertexId;
				float weight = aiBone->mWeights[w].mWeight;

				boneWeights[index].AddBoneWeight(boneIndex, weight);
			}
		}

		for (UINT w = 0; w < boneWeights.size(); w++)
		{
			boneWeights[w].Normalize();

			AssimpBlendWeight blendWeight;
			boneWeights[w].GetBlendWeiths(blendWeight);

			mesh->vertices[w].blendIndices = blendWeight.indices;
			mesh->vertices[w].blendWeights = blendWeight.weights;
		}
	}
}

void AssimpReader::WriteMesh(string saveFile, bool isOverwrite)
{
	if (!isOverwrite)
	{
		if (Utility::IsExistFile(saveFile))
			return;
	}

	Utility::CreateFolders(saveFile, true);

	BinaryWriter* w = new BinaryWriter(Utility::ToWString(saveFile));
	
	w->UInt(bones.size());
	for (AssimpBone* bone : bones)
	{
		w->Int(bone->index);
		w->String(bone->name);
		w->Int(bone->parent);
		w->Matrix(bone->transform);

		delete bone;
	}

	w->UInt(meshes.size());
	for (AssimpMesh* mesh : meshes)
	{
		w->String(mesh->name);
		w->Int(mesh->boneIndex);

		w->String(mesh->materialName);

		w->UInt(mesh->vertices.size());
		w->Byte(mesh->vertices.data(), sizeof(ModelVertexType) * mesh->vertices.size());
		w->UInt(mesh->indices.size());
		w->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());

		delete mesh;
	}

	delete w;
}

AssimpClip* AssimpReader::ReadClipData(aiAnimation* animation)
{
	AssimpClip* clip = new AssimpClip();
	clip->name = animation->mName.C_Str();
	clip->frameRate = (float)animation->mTicksPerSecond;
	clip->frameCount = (UINT)animation->mDuration + 1;

	vector<AssimpClipNode> nodeInfos;
	for (UINT i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* aniNode = animation->mChannels[i];

		AssimpClipNode nodeInfo;
		nodeInfo.name = aniNode->mNodeName;

		UINT keyCount = max(aniNode->mNumPositionKeys, aniNode->mNumRotationKeys);
		keyCount = max(keyCount, aniNode->mNumScalingKeys);

		AssimpKeyFrameData frameData;
		for (UINT k = 0; k < keyCount; k++)
		{
			bool isFound = false;
			UINT t = nodeInfo.keyFrame.size();

			if (abs((float)aniNode->mPositionKeys[k].mTime - (float)t) <= D3DX_16F_EPSILON)
			{
				aiVectorKey key = aniNode->mPositionKeys[k];
				memcpy_s(&frameData.position, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));
				frameData.time = (float)aniNode->mPositionKeys[k].mTime;

				isFound = true;
			}

			if (abs((float)aniNode->mRotationKeys[k].mTime - (float)t) <= D3DX_16F_EPSILON)
			{
				aiQuatKey key = aniNode->mRotationKeys[k];

				frameData.rotation.x = key.mValue.x;
				frameData.rotation.y = key.mValue.y;
				frameData.rotation.z = key.mValue.z;
				frameData.rotation.w = key.mValue.w;

				frameData.time = (float)aniNode->mRotationKeys[k].mTime;

				isFound = true;
			}

			if (abs((float)aniNode->mScalingKeys[k].mTime - (float)t) <= D3DX_16F_EPSILON)
			{
				aiVectorKey key = aniNode->mScalingKeys[k];
				memcpy_s(&frameData.scale, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));
				frameData.time = (float)aniNode->mScalingKeys[k].mTime;

				isFound = true;
			}

			if (isFound)
				nodeInfo.keyFrame.push_back(frameData);
		}

		if (nodeInfo.keyFrame.size() < clip->frameCount)
		{
			UINT count = clip->frameCount - nodeInfo.keyFrame.size();
			AssimpKeyFrameData keyFrame = nodeInfo.keyFrame.back();

			for (UINT n = 0; n < count; n++)
				nodeInfo.keyFrame.push_back(keyFrame);
		}
		clip->duration = max(clip->duration, nodeInfo.keyFrame.back().time);

		nodeInfos.push_back(nodeInfo);
	}

	ReadKeyframeData(clip, scene->mRootNode, nodeInfos);

	return clip;
}

void AssimpReader::ReadKeyframeData(AssimpClip* clip, aiNode* node, vector<AssimpClipNode>& nodeInfos)
{
	AssimpKeyFrame* keyFrame = new AssimpKeyFrame();
	keyFrame->boneName = node->mName.C_Str();

	for (UINT i = 0; i < clip->frameCount; i++)
	{
		AssimpClipNode* clipNode = nullptr;
		for (UINT n = 0; n < nodeInfos.size(); n++)
		{
			if (nodeInfos[n].name == node->mName)
			{
				clipNode = &nodeInfos[n];
				break;
			}
		}

		AssimpKeyFrameData frameData;
		if (clipNode == nullptr)
		{
			Matrix transform(node->mTransformation[0]);
			D3DXMatrixTranspose(&transform, &transform);

			D3DXMatrixDecompose(&frameData.scale, &frameData.rotation, &frameData.position, &transform);
			frameData.time = (float)i;
		}
		else
		{
			frameData = clipNode->keyFrame[i];
		}
		keyFrame->transforms.push_back(frameData);
	}
	clip->keyFrames.push_back(keyFrame);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadKeyframeData(clip, node->mChildren[i], nodeInfos);
}

void AssimpReader::WriteClipData(AssimpClip* clip, string saveFile, bool isOverwrite)
{
	if (!isOverwrite)
	{
		if (Utility::IsExistFile(saveFile))
			return;
	}

	Utility::CreateFolders(saveFile, true);

	BinaryWriter* w = new BinaryWriter(Utility::ToWString(saveFile));

	w->String(clip->name);
	w->Float(clip->duration);
	w->Float(clip->frameRate);
	w->Int(clip->frameCount);

	w->UInt(clip->keyFrames.size());
	for (AssimpKeyFrame* frame : clip->keyFrames)
	{
		w->String(frame->boneName);

		w->UInt(frame->transforms.size());
		w->Byte(frame->transforms.data(), sizeof(AssimpKeyFrameData) * frame->transforms.size());

		delete frame;
	}

	delete clip;
	delete w;
}
