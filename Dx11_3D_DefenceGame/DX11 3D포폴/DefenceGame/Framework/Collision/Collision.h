#pragma once

class Collision : public Transform
{
protected:
	struct InstanceData
	{
		Matrix world;
		Color color;

		InstanceData()
		{
			D3DXMatrixIdentity(&world);
			color = Color(0.0f, 0.0f, 0.0f, 1.0f);
		}
	};

	Shader* shader;

	VertexBuffer* vertexBuffer;
	VertexBuffer* instanceBuffer;
	IndexBuffer* indexBuffer;

	UINT indexCount;

	vector<Transform*> transforms;

	vector<InstanceData> instanceDatas;

	InstanceData instanceRenderDatas[MAX_RAND_RENDER_SIZE];

	vector<bool> isRender;
public:
	Collision();
	virtual ~Collision();

	virtual bool IsCollision(Collision* collision) = 0;
	virtual bool IsCollision(IN Ray ray, OUT float* distance = nullptr,
		OUT Vector3* contact = nullptr) = 0;

	Transform* GetTransform(int index) { return transforms[index]; }
	void SetIsRender(int index, bool render) { isRender[index] = render; }
	bool GetIsRender(int index) { return isRender[index]; }
	void SetRenderData(int renderIndex, int dataIndex);

	void SetColor(int index, Color color);
	void UpdateTransforms();
	void UpdateBuffer();

	void Render();
	void InstancingRender(int count = mapWidth * mapHeight);

	//void SetColor(Color color) { colorBuffer->data.color = color; }

	//VertexBuffer* GetInstanceBuffer() { return instanceBuffer; };
};