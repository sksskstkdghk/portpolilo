#include "Framework.h"

SpawnBase::SpawnBase()
{
	CreateData();

	baseLVL = 1;

	summonCharacters = 0;

	scale = Vector3(3.0f, 1.0f, 3.0f);
}

SpawnBase::~SpawnBase()
{

}

bool SpawnBase::IncludeOtherDiscrimToRect(Vector3 point)
{
	Vector3 min = Vector3(position.x - 1.5f, 0, position.z - 1.5f);
	Vector3 max = Vector3(position.x + 1.5f, 0, position.z + 1.5f);

	if (IntersectRect(min, max, point))
		return true;

	return false;
}

void SpawnBase::CreateData()
{
	VertexType v[4];
	v[0].position = Vector3(-0.5f, 0.0f, -0.5f);
	v[1].position = Vector3(0.5f, 0.0f, -0.5f);
	v[2].position = Vector3(-0.5f, 0.0f, 0.5f);
	v[3].position = Vector3(0.5f, 0.0f, 0.5f);

	v[0].uv = Vector2(0.0f, 1.0f);
	v[1].uv = Vector2(1.0f, 1.0f);
	v[2].uv = Vector2(0.0f, 0.0f);
	v[3].uv = Vector2(1.0f, 0.0f);

	for(int i =0; i<4; i++)
		vertices.push_back(v[i]);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(1);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	vertexBuffer->UpdateBuffer(vertices.data(), vertices.size());
}

void SpawnBase::SetDraw()
{
	vertexBuffer->Set();
	indexBuffer->Set();

	IASetPT();

	if (tex)
		tex->Set(10);

	if(shader)
		shader->Set();

	SetVS();

	DC->DrawIndexed(indices.size(), 0, 0);
}
