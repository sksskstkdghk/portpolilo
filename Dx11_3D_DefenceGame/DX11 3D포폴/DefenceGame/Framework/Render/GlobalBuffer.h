#pragma once

class ViewProjectionBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix view;
		Matrix projection;
		Matrix invView;
	}data;

public:
	ViewProjectionBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		D3DXMatrixIdentity(&data.view);
		D3DXMatrixIdentity(&data.projection);
	}

	void SetView(Matrix value)
	{
		//D3DXMatrixTranspose(&data.invView, &value);
		D3DXMatrixTranspose(&data.view, &value);
		D3DXMatrixTranspose(&data.invView, &data.view);
	}

	void SetProjection(Matrix value)
	{
		D3DXMatrixTranspose(&data.projection, &value);
	}

	Matrix GetProjection() { return data.projection; }
	Matrix GetView() { return data.view; }
};

class WorldBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix world;
	}data;

public:
	WorldBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		D3DXMatrixIdentity(&data.world);
	}

	void SetWorld(Matrix value)
	{
		D3DXMatrixTranspose(&data.world, &value);
	}

	Matrix GetWorld() { return data.world; }
};

class ColorBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Color color;
	}data;

	ColorBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.color = Color(1, 1, 1, 1);
	}
};

class LightBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Vector3 direction;
		float specExp;

		Color ambient;

		int isSpecularMap;
		int isNormalMap;

		float padding[2];
	}data;

	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.direction = Vector3(-1, -1, 1);
		data.specExp = 8.0f;

		data.ambient = Color(0.1f, 0.1f, 0.1f, 1.0f);
		data.isSpecularMap = 1;
		data.isNormalMap = 1;
	}
};

class MaterialBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Color diffuse;
		Color specular;
		Color ambient;
	}data;

	MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.diffuse = Color(1, 1, 1, 1);
		data.specular = Color(1, 1, 1, 1);
		data.ambient = Color(1, 1, 1, 1);
	}
};

class BrushBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int type;
		Vector3 location;

		float range;
		Vector3 color;
	}data;

	BrushBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.type = 2;
		data.location = Vector3(0, 0, 0);

		data.range = 10.0f;
		data.color = Vector3(0, 1, 0);
	}
};

class BoneBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix bones[MAX_MODEL_BONE];

		UINT index;
		float padding[3];
	}data;
public:
	BoneBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		for (int i = 0; i < MAX_MODEL_BONE; i++)
		{
			D3DXMatrixIdentity(&data.bones[i]);
		}

		data.index = 0;
	}

	void Bones(Matrix* m, UINT count)
	{
		memcpy(data.bones, m, sizeof(Matrix) * count);

		for (UINT i = 0; i < count; i++)
			D3DXMatrixTranspose(&data.bones[i], &data.bones[i]);
	}

	void SetIndex(UINT value)
	{
		data.index = value;
	}
};

class IntBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int index;		

		float padding[3];
	}data;

	IntBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.index = 0;		
	}
};

class FloatBuffer : public ConstBuffer
{
public:
	struct Data
	{
		float index;

		float padding[3];
	}data;

	FloatBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.index = 0;
	}
};