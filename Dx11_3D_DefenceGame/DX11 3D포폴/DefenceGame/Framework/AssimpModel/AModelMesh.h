#pragma once

struct AModelBone;
class AModel;

class AModelMesh
{
private:
	friend class AModel;

	string name;

	string materialName;

	Material* material;
	BoneBuffer* boneBuffer;

	int boneIndex;
	AModelBone* bone;
	
	UINT vertexCount, indexCount;

	ModelVertexType* vertices;
	UINT* indices;

	VertexBuffer* vertexBuffer;	
	IndexBuffer* indexBuffer;

	ID3D11ShaderResourceView* transformsSRV;

	AModelMesh();
	~AModelMesh();

	void Binding(AModel* model);
public:
	void Update();
	void Render(UINT drawCount);

	void SetShader(Shader* shader);
	void SetTransforms(Matrix* transforms);	
	void SetTransformsSRV(ID3D11ShaderResourceView* srv) { transformsSRV = srv; }

	int GetBoneIndex() { return boneIndex; }	
};