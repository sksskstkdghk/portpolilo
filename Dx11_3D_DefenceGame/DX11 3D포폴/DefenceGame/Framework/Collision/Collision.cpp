#include "Framework.h"

Collision::Collision()
{
	shader = Shader::Add(L"Collision");
	//colorBuffer = new ColorBuffer();
	//colorBuffer->data.color = Color(0, 1, 0, 1);

}

Collision::~Collision()
{
	//delete colorBuffer;
	delete vertexBuffer;
	delete indexBuffer;
}

void Collision::SetRenderData(int renderIndex, int dataIndex)
{
	memcpy(&instanceRenderDatas[renderIndex], &instanceDatas[dataIndex], sizeof(InstanceData));

	//instanceBuffer->\(instanceRenderDatas, MAX_RAND_RENDER_SIZE);
}

void Collision::SetColor(int index, Color color)
{
	instanceDatas[index].color = color;
}

void Collision::UpdateBuffer()
{
	instanceBuffer->UpdateBuffer(instanceRenderDatas, MAX_RAND_RENDER_SIZE);
}

void Collision::UpdateTransforms()
{
	for (int i = 0; i < transforms.size(); i++)
	{
		transforms[i]->UpdateWorld();
		memcpy(&instanceDatas[i].world, &transforms[i]->GetTransposeWorld(), sizeof(Matrix));
	}
}

void Collision::Render()
{
	UpdateWorld();

	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//colorBuffer->SetPSBuffer(10);
	SetVS();

	shader->Set();

	DC->DrawIndexed(indexCount, 0, 0);
}

void Collision::InstancingRender(int count)
{
	//UpdateWorld();

	instanceBuffer->Set(1);
	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//colorBuffer->SetPSBuffer(10);
	//SetVS();

	shader->Set();

	DC->DrawIndexedInstanced(indexCount, count, 0, 0, 0);
}
