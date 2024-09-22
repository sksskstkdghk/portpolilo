#pragma once

typedef VertexUVNormalTangentBlend ModelVertexType;

struct FbxKeyFrameData
{
	float time;

	Vector3 scale;
	Quaternion rotation;
	Vector3 position;
};

struct FbxKeyFrame
{
	string boneName;
	vector<FbxKeyFrameData> transforms;
};

struct FbxClip
{
	string name;

	int frameCount;
	float frameRate;
	float duration;

	vector<FbxKeyFrame*> keyFrames;
};

struct FbxAnimation
{
	string name;

	int totalFrame;
	float frameRate;

	vector<FbxKeyFrame*> keyFrames;
};

//struct FbxBlendWeight
//{
//	Vector4 indices = Vector4(0, 0, 0, 0);
//	Vector4 weights = Vector4(0, 0, 0, 0);
//
//	void Set(UINT index, UINT boneIndex, float weight)
//	{
//		float i = (float)boneIndex;
//		float w = weight;
//
//		switch (index)
//		{
//		case 0:
//			indices.x = i;
//			weights.x = w;
//			break;
//		case 1:
//			indices.y = i;
//			weights.y = w;
//			break;
//		case 2:
//			indices.z = i;
//			weights.z = w;
//			break;
//		case 3:
//			indices.w = i;
//			weights.w = w;
//			break;
//		default:
//			break;
//		}
//	}
//};
//
//struct FbxBoneWeights
//{
//private:
//	typedef pair<UINT, float> Pair;
//	vector<Pair> boneWeights;
//
//public:
//	void AddBoneWeight(UINT boneIndex, float boneWeight)
//	{
//		if (boneWeight <= 0.0f)
//			return;
//
//		bool isAdd = false;
//
//		auto it = boneWeights.begin();
//		while (it != boneWeights.end())
//		{
//			if (boneWeight > it->second)
//			{
//				boneWeights.insert(it, Pair(boneIndex, boneWeight));
//				isAdd = true;
//				break;
//			}
//			it++;
//		}
//
//		if (!isAdd)
//			boneWeights.push_back(Pair(boneIndex, boneWeight));
//	}
//
//	void GetBlendWeiths(FbxBlendWeight& blendWeights)
//	{
//		for (UINT i = 0; i < boneWeights.size(); i++)
//		{
//			if (i >= 4)
//				return;
//
//			blendWeights.Set(i, boneWeights[i].first, boneWeights[i].second);
//		}
//	}
//
//	void Normalize()
//	{
//		float totalWeight = 0.0f;
//
//		int i = 0;
//
//		auto it = boneWeights.begin();
//
//		while (it != boneWeights.end())
//		{
//			if (i < 4)
//			{
//				totalWeight += it->second;
//
//				i++;
//				it++;
//			}
//			else
//				it = boneWeights.erase(it);
//		}
//
//		float scale = 1.0f / totalWeight;
//
//		it = boneWeights.begin();
//		while (it != boneWeights.end())
//		{
//			it->second *= scale;
//			it++;
//		}
//	}
//};

///////////////////////////////////////////////////////////////

struct AssimpMaterial
{
	string name;

	Color ambient;
	Color diffuse;
	Color specular;

	string diffuseFile;
	string specularFile;
	string normalFile;
};

struct AssimpBone
{
	int index;
	string name;

	int parent;
	Matrix transform;
};

//struct AssimpMeshPart
//{
//	string name;
//	string materialName;
//
//	UINT startVertex;
//	UINT vertexCount;
//
//	UINT startIndex;
//	UINT indexCount;
//};

struct AssimpMesh
{
	string name;
	int boneIndex;

	aiMesh* mesh;

	string materialName;

	vector<ModelVertexType> vertices;
	vector<UINT> indices;
};

struct AssimpBlendWeight
{
	Vector4 indices = Vector4(0, 0, 0, 0);
	Vector4 weights = Vector4(0, 0, 0, 0);

	void Set(UINT index, UINT boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (index)
		{
		case 0:
			indices.x = i;
			weights.x = w;
			break;
		case 1:
			indices.y = i;
			weights.y = w;
			break;
		case 2:
			indices.z = i;
			weights.z = w;
			break;
		case 3:
			indices.w = i;
			weights.w = w;
			break;
		default:
			break;
		}
	}
};

struct AssimpBoneWeights
{
private:
	typedef pair<UINT, float> Pair;
	vector<Pair> boneWeights;

public:
	void AddBoneWeight(UINT boneIndex, float boneWeight)
	{
		if (boneWeight <= 0.0f)
			return;

		bool isAdd = false;

		auto it = boneWeights.begin();
		while (it != boneWeights.end())
		{
			if (boneWeight > it->second)
			{
				boneWeights.insert(it, Pair(boneIndex, boneWeight));
				isAdd = true;
				break;
			}
			it++;
		}

		if (!isAdd)
			boneWeights.push_back(Pair(boneIndex, boneWeight));
	}

	void GetBlendWeiths(AssimpBlendWeight& blendWeights)
	{
		for (UINT i = 0; i < boneWeights.size(); i++)
		{
			if (i >= 4)
				return;

			blendWeights.Set(i, boneWeights[i].first, boneWeights[i].second);
		}
	}

	void Normalize()
	{
		float totalWeight = 0.0f;

		int i = 0;

		auto it = boneWeights.begin();

		while (it != boneWeights.end())
		{
			if (i < 4)
			{
				totalWeight += it->second;

				i++;
				it++;
			}
			else
				it = boneWeights.erase(it);
		}

		float scale = 1.0f / totalWeight;

		it = boneWeights.begin();
		while (it != boneWeights.end())
		{
			it->second *= scale;
			it++;
		}
	}
};

struct AssimpKeyFrameData
{
	float time;

	Vector3 scale;
	Quaternion rotation;
	Vector3 position;
};

struct AssimpKeyFrame
{
	string boneName;
	vector<AssimpKeyFrameData> transforms;
};

struct AssimpClipNode
{
	vector<AssimpKeyFrameData> keyFrame;
	aiString name;
};

struct AssimpClip
{
	string name;

	UINT frameCount;
	float frameRate;
	float duration;

	vector<AssimpKeyFrame*> keyFrames;
};