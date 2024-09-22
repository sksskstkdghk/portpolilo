#include "Framework.h"

SphereCollision::SphereCollision(float radius, UINT stackCount, UINT sliceCount)
	: radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	Create(mapWidth * mapHeight);
}

SphereCollision::~SphereCollision()
{
	transforms.clear();
	transforms.shrink_to_fit();

	instanceDatas.clear();
	instanceDatas.shrink_to_fit();
}

bool SphereCollision::IsCollision(Collision* collision)
{
	SphereCollision* coll = dynamic_cast<SphereCollision*>(collision);

	return IsCollision(coll);
}

bool SphereCollision::IsCollision(IN Ray ray, OUT float* distance, OUT Vector3* contact)
{
	Vector3 P = ray.position;
	Vector3 D = ray.direction;

	Vector3 C = worldPosition;
	Vector3 A = P - C;

	float a = D3DXVec3Dot(&D, &D);
	float b = 2 * D3DXVec3Dot(&A, &D);
	float c = D3DXVec3Dot(&A, &A) - radius * radius;

	if (b * b >= 4 * a * c)
	{
		if (distance != nullptr)
		{
			*distance = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

			if (contact != nullptr)
			{
				*contact = P + *distance * D;
			}
		}
		else
		{			
			if(contact != nullptr)
			{
				float temp = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
				*contact = P + temp * D;
			}
		}
		return true;
	}

	return false;
}

void SphereCollision::Create(UINT size)
{
	vector<Vertex> vertices;
	vector<UINT> indices;

	float phiStep = D3DX_PI / stackCount;
	float thetaStep = 2.0f * D3DX_PI / sliceCount;

	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			Vector3 normal;

			normal.x = sin(phi) * cos(theta);
			normal.y = cos(phi);
			normal.z = sin(phi) * sin(theta);

			Vertex vertex;
			vertex.position = normal * radius;

			vertices.push_back(vertex);
		}
	}

	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.push_back((sliceCount + 1) * i + j);//0
			indices.push_back((sliceCount + 1) * i + j + 1);//1			

			indices.push_back((sliceCount + 1) * i + j);//0
			indices.push_back((sliceCount + 1) * (i + 1) + j);//2
		}
	}

	for (int i = 0; i < size; i++)
	{
		Transform* temp = new Transform();
		transforms.push_back(temp);
	}

	instanceDatas.resize(size);
	isRender.resize(size);

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(Vertex), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
	instanceBuffer = new VertexBuffer(instanceRenderDatas, sizeof(InstanceData), MAX_RAND_RENDER_SIZE);

	indexCount = indices.size();
}

bool SphereCollision::IsCollision(SphereCollision* collision)
{
	float distance = Distance(worldPosition, collision->worldPosition);

	return distance <= (radius + collision->radius);
}
