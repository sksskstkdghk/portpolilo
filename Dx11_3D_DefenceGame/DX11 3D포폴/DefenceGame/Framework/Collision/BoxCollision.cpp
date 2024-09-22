#include "Framework.h"

BoxCollision::BoxCollision(Vector3 min, Vector3 max)
	: minBox(min), maxBox(max)
{
	Create(mapWidth * mapHeight);
}

BoxCollision::~BoxCollision()
{
	transforms.clear();
	transforms.shrink_to_fit();

	instanceDatas.clear();
	instanceDatas.shrink_to_fit();
}

bool BoxCollision::IsCollision(Collision* collision)
{
	BoxCollision* coll = reinterpret_cast<BoxCollision*>(collision);
	
	return OBB(coll);
}

bool BoxCollision::IsCollision(IN Ray ray, OUT float* distance, OUT Vector3* contact)
{
	UINT indices[24]
	{
		0, 1, 2, 3,
		0, 1, 5, 4,
		4, 5, 6, 7,
		2, 6, 7, 3,
		1, 5, 6, 2,
		0, 4, 7, 3
	};

	Vector3 minContact;
	float minDistance = -1;

	for (UINT i = 0; i < 6; i++)
	{
		Vector3 p[4];

		p[0] = vertices[indices[i * 4 + 0]].position;
		p[1] = vertices[indices[i * 4 + 1]].position;
		p[2] = vertices[indices[i * 4 + 2]].position;
		p[3] = vertices[indices[i * 4 + 3]].position;

		D3DXVec3TransformCoord(&p[0], &p[0], &world);
		D3DXVec3TransformCoord(&p[1], &p[1], &world);
		D3DXVec3TransformCoord(&p[2], &p[2], &world);
		D3DXVec3TransformCoord(&p[3], &p[3], &world);

		float u, v, dist;
		if (D3DXIntersectTri(&p[0], &p[1], &p[3], &ray.position,
			&ray.direction, &u, &v, &dist))
		{
			if (minDistance == -1 || dist < minDistance)
			{
				minDistance = dist;
				minContact = p[0] + (p[1] - p[0]) * u + (p[3] - p[0]) * v;
			}
		}

		if (D3DXIntersectTri(&p[2], &p[1], &p[3], &ray.position,
			&ray.direction, &u, &v, &dist))
		{
			if (minDistance == -1 || dist < minDistance)
			{
				minDistance = dist;
				minContact = p[2] + (p[1] - p[2]) * u + (p[3] - p[2]) * v;
			}
		}
	}

	if (minDistance == -1)
		return false;

	if (distance != nullptr)
		*distance = minDistance;

	if (contact != nullptr)
		*contact = minContact;	

	return true;
}

bool BoxCollision::IsInstanceCollision(IN Ray ray, int index, OUT float* distance, OUT Vector3* contact)
{
	UINT indices[24]
	{
		0, 1, 2, 3,
		0, 1, 5, 4,
		4, 5, 6, 7,
		2, 6, 7, 3,
		1, 5, 6, 2,
		0, 4, 7, 3
	};

	Vector3 minContact;
	float minDistance = -1;

	for (UINT i = 0; i < 6; i++)
	{
		Vector3 p[4];

		p[0] = vertices[indices[i * 4 + 0]].position;
		p[1] = vertices[indices[i * 4 + 1]].position;
		p[2] = vertices[indices[i * 4 + 2]].position;
		p[3] = vertices[indices[i * 4 + 3]].position;

		D3DXVec3TransformCoord(&p[0], &p[0], &transforms[index]->GetWorld());
		D3DXVec3TransformCoord(&p[1], &p[1], &transforms[index]->GetWorld());
		D3DXVec3TransformCoord(&p[2], &p[2], &transforms[index]->GetWorld());
		D3DXVec3TransformCoord(&p[3], &p[3], &transforms[index]->GetWorld());

		float u, v, dist;
		if (D3DXIntersectTri(&p[0], &p[1], &p[3], &ray.position,
			&ray.direction, &u, &v, &dist))
		{
			if (minDistance == -1 || dist < minDistance)
			{
				minDistance = dist;
				minContact = p[0] + (p[1] - p[0]) * u + (p[3] - p[0]) * v;
			}
		}

		if (D3DXIntersectTri(&p[2], &p[1], &p[3], &ray.position,
			&ray.direction, &u, &v, &dist))
		{
			if (minDistance == -1 || dist < minDistance)
			{
				minDistance = dist;
				minContact = p[2] + (p[1] - p[2]) * u + (p[3] - p[2]) * v;
			}
		}
	}

	if (minDistance == -1)
		return false;

	if (distance != nullptr)
		*distance = minDistance;

	if (contact != nullptr)
		*contact = minContact;

	return true;
}

Vector3 BoxCollision::GetMin()
{
	Vector3 temp;
	D3DXVec3TransformCoord(&temp, &minBox, &world);

	return temp;
}

Vector3 BoxCollision::GetMax()
{
	Vector3 temp;
	D3DXVec3TransformCoord(&temp, &maxBox, &world);

	return temp;
}

void BoxCollision::Create(UINT size)
{
	vertices[0] = Vertex(minBox.x, minBox.y, minBox.z);
	vertices[1] = Vertex(minBox.x, maxBox.y, minBox.z);
	vertices[2] = Vertex(maxBox.x, maxBox.y, minBox.z);
	vertices[3] = Vertex(maxBox.x, minBox.y, minBox.z);

	vertices[4] = Vertex(minBox.x, minBox.y, maxBox.z);
	vertices[5] = Vertex(minBox.x, maxBox.y, maxBox.z);
	vertices[6] = Vertex(maxBox.x, maxBox.y, maxBox.z);
	vertices[7] = Vertex(maxBox.x, minBox.y, maxBox.z);

	UINT indices[24]
	{
		0, 3, 1, 2, 5, 6, 4, 7,
		0, 4, 1, 5, 2, 6, 3, 7,
		0, 1, 3, 2, 7, 6, 4, 5
	};

	instanceDatas.resize(size);
	isRender.resize(size);

	for (int i = 0; i < size; i++)
	{
		Transform* temp = new Transform();
		transforms.push_back(temp);

		instanceDatas[i].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
		isRender[i] = true;
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(Vertex), 8);
	indexBuffer = new IndexBuffer(indices, 24);
	instanceBuffer = new VertexBuffer(instanceRenderDatas, sizeof(InstanceData), MAX_RAND_RENDER_SIZE);

	indexCount = 24;
}

bool BoxCollision::AABB(BoxCollision* collision)
{
	Vector3 box1Min = GetMin();
	Vector3 box1Max = GetMax();

	Vector3 box2Min = collision->GetMin();
	Vector3 box2Max = collision->GetMax();

	float x1 = max(box1Min.x, box2Min.x);
	float x2 = min(box1Max.x, box2Max.x);

	float y1 = max(box1Min.y, box2Min.y);
	float y2 = min(box1Max.y, box2Max.y);

	float z1 = max(box1Min.z, box2Min.z);
	float z2 = min(box1Max.z, box2Max.z);

	if (x2 - x1 > 0 && y2 - y1 > 0 && z2 - z1 > 0)
		return true;

	return false;
}

bool BoxCollision::OBB(BoxCollision* collision)
{
	Obb box1 = GetObb();
	Obb box2 = collision->GetObb();

	Vector3 position = box2.position - box1.position;

	if (SeperateAxis(position, box1.axisX, box1, box2)) return false;
	if (SeperateAxis(position, box1.axisY, box1, box2)) return false;
	if (SeperateAxis(position, box1.axisZ, box1, box2)) return false;

	if (SeperateAxis(position, box2.axisX, box1, box2)) return false;
	if (SeperateAxis(position, box2.axisY, box1, box2)) return false;
	if (SeperateAxis(position, box2.axisZ, box1, box2)) return false;

	if (SeperateAxis(position, Cross(box1.axisX, box2.axisX), box1, box2)) return false;
	if (SeperateAxis(position, Cross(box1.axisX, box2.axisY), box1, box2)) return false;
	if (SeperateAxis(position, Cross(box1.axisX, box2.axisZ), box1, box2)) return false;

	if (SeperateAxis(position, Cross(box1.axisY, box2.axisX), box1, box2)) return false;
	if (SeperateAxis(position, Cross(box1.axisY, box2.axisY), box1, box2)) return false;
	if (SeperateAxis(position, Cross(box1.axisY, box2.axisZ), box1, box2)) return false;

	if (SeperateAxis(position, Cross(box1.axisZ, box2.axisX), box1, box2)) return false;
	if (SeperateAxis(position, Cross(box1.axisZ, box2.axisY), box1, box2)) return false;
	if (SeperateAxis(position, Cross(box1.axisZ, box2.axisZ), box1, box2)) return false;

	return true;
}

Obb BoxCollision::GetObb()
{
	Obb obb;

	obb.position = worldPosition;

	obb.axisX = right;
	obb.axisY = up;
	obb.axisZ = forward;

	obb.halfSize = (maxBox - minBox) * 0.5f;
	obb.halfSize.x *= scale.x;
	obb.halfSize.y *= scale.y;
	obb.halfSize.z *= scale.z;

	return obb;
}

bool BoxCollision::SeperateAxis(Vector3 position, Vector3 direction, Obb box1, Obb box2)
{
	float d = abs(D3DXVec3Dot(&position, &direction));

	float a = 0.0f;
	float b = 0.0f;

	a += abs(D3DXVec3Dot(&(box1.axisX * box1.halfSize.x), &direction));
	a += abs(D3DXVec3Dot(&(box1.axisY * box1.halfSize.y), &direction));
	a += abs(D3DXVec3Dot(&(box1.axisZ * box1.halfSize.z), &direction));

	b += abs(D3DXVec3Dot(&(box2.axisX * box2.halfSize.x), &direction));
	b += abs(D3DXVec3Dot(&(box2.axisY * box2.halfSize.y), &direction));
	b += abs(D3DXVec3Dot(&(box2.axisZ * box2.halfSize.z), &direction));

	return d > a + b;
}