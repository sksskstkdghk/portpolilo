#pragma once

class ModelMesh;

class ModelMeshPart
{
private:
	friend class Model;
	friend class ModelMesh;

	Material* material;
	string materialName;

	ModelMesh* parent;

	vector<ModelVertexType> vertices;
	vector<UINT> indices;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	ModelMeshPart();
	~ModelMeshPart();

	void Render();

	void Binding();
};