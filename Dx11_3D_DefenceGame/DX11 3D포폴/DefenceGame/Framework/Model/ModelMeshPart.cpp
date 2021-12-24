#include "Framework.h"

ModelMeshPart::ModelMeshPart()
{
}

ModelMeshPart::~ModelMeshPart()
{
}

void ModelMeshPart::Render()
{
	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void ModelMeshPart::Binding()
{
	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(ModelVertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}
