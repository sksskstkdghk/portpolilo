#pragma once

struct Vertex
{
	Vector3 position;

	Vertex()
	{
		position = Vector3(0, 0, 0);
	}
	Vertex(float x, float y, float z)
	{
		position = Vector3(x, y, z);
	}
};

struct VertexUV
{
	Vector3 position;
	Vector2 uv;

	VertexUV()
	{
		position = Vector3(0, 0, 0);
		uv = Vector2(0, 0);
	}
};

struct VertexUVColor
{
	Vector3 position;
	Vector2 uv;
	Color color;

	VertexUVColor()
	{
		position = Vector3(0, 0, 0);
		uv = Vector2(0, 0);
		color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	}
};

struct VertexSize
{
	Vector3 position;
	Vector2 size;

	VertexSize()
	{
		position = Vector3(0, 0, 0);
		size = Vector2(0, 0);
	}
};

struct VertexUVNormal
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;

	VertexUVNormal()
	{
		position = Vector3(0, 0, 0);
		uv = Vector2(0, 0);
		normal = Vector3(0, 0, 0);
	}
};

struct VertexUVNormalTangent
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;

	VertexUVNormalTangent()
	{
		position = Vector3(0, 0, 0);
		uv = Vector2(0, 0);
		normal = Vector3(0, 0, 0);
		tangent = Vector3(0, 0, 0);
	}
};

struct VertexUVNormalTangentColor
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
	Color color;

	VertexUVNormalTangentColor()
	{
		position = Vector3(0, 0, 0);
		uv = Vector2(0, 0);
		normal = Vector3(0, 0, 0);
		tangent = Vector3(0, 0, 0);
		color = Color(0, 0, 0, 1);
	}
};

struct VertexUVNormalTangentBlend
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
	Vector4 blendIndices;
	Vector4 blendWeights;

	VertexUVNormalTangentBlend()
	{
		position = Vector3(0, 0, 0);
		uv = Vector2(0, 0);
		normal = Vector3(0, 0, 0);
		tangent = Vector3(0, 0, 0);
		blendIndices = Vector4(0, 0, 0, 0);
		blendWeights = Vector4(0, 0, 0, 0);
	}
};
