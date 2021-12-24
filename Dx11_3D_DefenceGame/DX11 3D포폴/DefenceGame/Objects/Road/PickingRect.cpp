#include "Framework.h"
#include "PickingRect.h"

PickingRect::PickingRect(int width, int height)
{
	vertices.push_back(VertexType(0.0f, 0.0f, 0.0f));
	vertices.push_back(VertexType(0.0f, 0.0f, height));
	vertices.push_back(VertexType(width, 0.0f, height));
	vertices.push_back(VertexType(width, 0.0f, 0.0f));
}

PickingRect::~PickingRect()
{
	vertices.clear();
	vertices.shrink_to_fit();
}

bool PickingRect::Picking(Vector3* position)
{
	Ray ray = CAMERA->GetPickingRay();

	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
		p[i] = vertices.at(i).position;

	float u, v, distance;
	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &ray.position, &ray.direction,
		&u, &v, &distance))
	{

		*position = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
		return true;
	}

	if (D3DXIntersectTri(&p[0], &p[2], &p[3], &ray.position, &ray.direction,
		&u, &v, &distance))
	{
		*position = p[0] + (p[2] - p[0]) * u + (p[3] - p[0]) * v;
		return true;
	}

	return false;
}
