#include "Framework.h"

AModelRender::AModelRender(Shader* shader, string name)
{
	model = new AModel();
	model->ReadMaterial(name + ".mat", name + "/");
	model->ReadMesh(name + ".mesh", name + "/");

	for (AModelMesh* mesh : *model->GetMeshes())
		mesh->SetShader(shader);

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		D3DXMatrixIdentity(&worlds[i]);

	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE);
}

AModelRender::~AModelRender()
{
	for (Transform* t : transforms)
		delete t;

	texture->Release();
	srv->Release();

	delete instanceBuffer;
	delete model;
}

void AModelRender::Update()
{
	for (AModelMesh* mesh : *model->GetMeshes())
		mesh->Update();

	//UpdateWorld();
}

void AModelRender::Render()
{
	if (texture == nullptr)
		CreateTexture();

	instanceBuffer->Set(1);

	for (AModelMesh* mesh : *model->GetMeshes())
	{
		mesh->Render(transforms.size());
	}
}

void AModelRender::UpdateTransform(UINT instanceID, UINT boneIndex, Transform& transform)
{
	Matrix destMatrix = transform.GetTransposeWorld();

	AModelBone* bone = model->GetBones()->at(boneIndex);
	boneTransforms[instanceID][boneIndex] = destMatrix * boneTransforms[instanceID][boneIndex];

	UINT tempBoneIndex = boneIndex;
	for (AModelBone* child : bone->children)
	{
		Matrix parent = boneTransforms[instanceID][boneIndex];

		Matrix invParent;
		D3DXMatrixInverse(&invParent, nullptr, &parent);
		tempBoneIndex++;

		Matrix temp = boneTransforms[instanceID][tempBoneIndex] * invParent;
		boneTransforms[instanceID][tempBoneIndex] = temp * destMatrix * parent;
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	memcpy(subResource.pData, boneTransforms,
		MAX_MODEL_BONE * MAX_MODEL_INSTANCE * sizeof(Matrix));
	DC->Unmap(texture, 0);
}

void AModelRender::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
		memcpy(&worlds[i], &transforms[i]->GetTransposeWorld(), sizeof(Matrix));

	instanceBuffer->UpdateBuffer(worlds, MAX_MODEL_INSTANCE);
}

Transform* AModelRender::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);

	return transform;
}

void AModelRender::CreateTexture()
{
	{//CreateTexture
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = MAX_MODEL_BONE * 4;
		desc.Height = MAX_MODEL_INSTANCE;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		{
			for (UINT b = 0; b < model->GetBones()->size(); b++)
			{
				AModelBone* bone = model->GetBones()->at(b);

				Matrix parent;
				int parentIndex = bone->parentIndex;

				if (parentIndex < 0)
					D3DXMatrixIdentity(&parent);
				else
					parent = bones[parentIndex];

				Matrix matrix = bone->transform;
				bones[b] = parent;
				boneTransforms[i][b] = matrix * bones[b];
			}
		}

		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = boneTransforms;
		subResource.SysMemPitch = MAX_MODEL_BONE * sizeof(Matrix);
		subResource.SysMemSlicePitch = MAX_MODEL_BONE * sizeof(Matrix) * MAX_MODEL_INSTANCE;

		DEVICE->CreateTexture2D(&desc, &subResource, &texture);
	}

	{//CreateSRV
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = 1;

		DEVICE->CreateShaderResourceView(texture, &desc, &srv);
	}

	for (AModelMesh* mesh : *model->GetMeshes())
		mesh->SetTransformsSRV(srv);
}