#include "Framework.h"
#include "BoxWall.h"

BoxWall::BoxWall()
{
	material = new Material(L"TerrainBrush");
	//material = new Material(L"Splatting");
	material->SetDiffuseMap(L"Textures/Landscape/Bricks.png");
	//material->SetSpecularMap(L"Textures/Landscape/Wall_specular.png");
	material->SetNormalMap(L"Textures/Landscape/Bricks_normal.png");

	CreateData();

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

}

BoxWall::~BoxWall()
{
	delete vertexBuffer;
	delete indexBuffer;

	delete material;
}

void BoxWall::Render(VertexBuffer* instanceBuffer, UINT count)
{
	//UpdateWorld();

	instanceBuffer->Set(1);
	vertexBuffer->Set();
	indexBuffer->Set();

	IASetPT();
	material->Set();

	DC->DrawIndexedInstanced(indices.size(), count, 0, 0, 0);
}

void BoxWall::CreateData()
{
	{//Terrain edit
		{//Vertices

			for (UINT y = 0; y < 2; y++)
			{
				for (UINT x = 0; x < 2; x++)
				{
					VertexType vertex;
					vertex.position = Vector3(x, y, 0);
					vertex.uv = Vector2(x, y);

					vertices.push_back(vertex);
				}
			}

			for (UINT y = 0; y < 2; y++)
			{
				for (UINT x = 0; x < 2; x++)
				{
					VertexType vertex;
					vertex.position = Vector3(x, y, 1);
					vertex.uv = Vector2(1 - x, y);

					vertices.push_back(vertex);
				}
			}

			for (UINT y = 0; y < 2; y++)
			{
				for (UINT x = 0; x < 2; x++)
				{
					VertexType vertex;
					vertex.position = Vector3(x, y, 1);
					vertex.uv = Vector2(x, 1 - y);

					vertices.push_back(vertex);
				}
			}
		}

		{//Indices ¾Õ¸é
			indices.push_back(0);//0
			indices.push_back(3);//1
			indices.push_back(1);//2

			indices.push_back(0);//0				
			indices.push_back(2);//2
			indices.push_back(3);//3
		}

		{//Indices ¿ÞÂÊ¸é
			indices.push_back(4);//0
			indices.push_back(6);//1
			indices.push_back(0);//2

			indices.push_back(6);//0				
			indices.push_back(2);//2
			indices.push_back(0);//3
		}

		{//Indices ¿À¸¥ÂÊ ¸é
			indices.push_back(1);//0
			indices.push_back(7);//1
			indices.push_back(5);//2

			indices.push_back(1);//0				
			indices.push_back(3);//2
			indices.push_back(7);//3
		}

		{//Indices µÞ¸é
			indices.push_back(7);//0
			indices.push_back(6);//1
			indices.push_back(4);//2

			indices.push_back(5);//0				
			indices.push_back(7);//2
			indices.push_back(4);//3
		}

		{//Indices ¾Æ·§¸é
			indices.push_back(0);//0
			indices.push_back(9);//1
			indices.push_back(1);//2

			indices.push_back(0);//0				
			indices.push_back(8);//2
			indices.push_back(9);//3
		}

		{//Indices À­ ¸é
			indices.push_back(2);//0
			indices.push_back(10);//1
			indices.push_back(11);//2

			indices.push_back(2);//0				
			indices.push_back(11);//2
			indices.push_back(3);//3
		}
	}

	CreateNormal();
	CreateTangent();
}

void BoxWall::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType v0 = vertices[index0];
		VertexType v1 = vertices[index1];
		VertexType v2 = vertices[index2];

		Vector3 A = v1.position - v0.position;
		Vector3 B = v2.position - v0.position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &A, &B);

		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}

	for (VertexType& vertex : vertices)
		D3DXVec3Normalize(&vertex.normal, &vertex.normal);
}

void BoxWall::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = vertices[index0];
		VertexType vertex1 = vertices[index1];
		VertexType vertex2 = vertices[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		Vector2 uv0 = vertex0.uv;
		Vector2 uv1 = vertex1.uv;
		Vector2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);

		Vector3 tangent;
		tangent = (v1 * e0 - v0 * e1) * d;

		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}

	for (VertexType& vertex : vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		Vector3 temp = (t - n * D3DXVec3Dot(&n, &t));
		D3DXVec3Normalize(&temp, &temp);

		vertex.tangent = temp;
	}
}