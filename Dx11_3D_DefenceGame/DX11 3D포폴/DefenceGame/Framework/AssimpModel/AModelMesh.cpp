#include "Framework.h"

AModelMesh::AModelMesh(): transformsSRV(nullptr)
{
	boneBuffer = new BoneBuffer();
}

AModelMesh::~AModelMesh()
{
	delete boneBuffer;

	delete vertexBuffer;
	delete indexBuffer;

	delete[] vertices;
	delete[] indices;
}

void AModelMesh::Binding(AModel* model)
{
	vertexBuffer = new VertexBuffer(vertices, sizeof(ModelVertexType), vertexCount);
	indexBuffer = new IndexBuffer(indices, indexCount);

	Material* srcMaterial = model->MaterialByName(materialName);

	material = new Material();
	material->GetBuffer()->data = srcMaterial->GetBuffer()->data;

	if (srcMaterial->GetDiffuseMap() != nullptr)
		material->SetDiffuseMap(srcMaterial->GetDiffuseMap()->GetFile());

	if (srcMaterial->GetSpecularMap() != nullptr)
		material->SetSpecularMap(srcMaterial->GetSpecularMap()->GetFile());

	if (srcMaterial->GetNormalMap() != nullptr)
		material->SetNormalMap(srcMaterial->GetNormalMap()->GetFile());
}

void AModelMesh::Update()
{
	boneBuffer->SetIndex(boneIndex);
}

void AModelMesh::Render(UINT drawCount)
{
	vertexBuffer->Set();
	indexBuffer->Set();

	IASetPT();

	if (transformsSRV != nullptr)
		DC->VSSetShaderResources(10, 1, &transformsSRV);

	//boneBuffer->SetVSBuffer(2);

	material->Set();

	DC->DrawIndexedInstanced(indexCount, drawCount, 0, 0, 0);
}

void AModelMesh::SetShader(Shader* shader)
{
	material->SetShader(shader);
}

void AModelMesh::SetTransforms(Matrix* transforms)
{
	boneBuffer->Bones(transforms, MAX_MODEL_BONE);
}
