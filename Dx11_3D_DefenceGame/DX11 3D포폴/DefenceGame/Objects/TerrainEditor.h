#pragma once

class SpawnBase;
class Render2D;

class TerrainEditor
{
private:	//���� �׸��� ���� ��
	enum SelectType
	{
		TYPE_NONE = 0,
		TYPE_ROAD, TYPE_FRIENDLY, TYPE_DUNGEON
	};

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

	typedef VertexUVNormalTangent VertexType;

	UINT width, height;

	Material* material;

	vector<InstanceData> instanceDatas;

	InstanceData instanceRenderDatas[MAX_RAND_RENDER_SIZE];
	int instanceRenderDatasIndex[MAX_RAND_RENDER_SIZE];

	VertexBuffer* instanceBuffer;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	vector<VertexType> vertices;
	vector<UINT> indices;

	int count;

private:	//�׸� ���� �����ϴ� ��
	//������ �簢������ �з��ؼ� ����ϱ� ���� �뵵
	vector<RectRoad*> rectDatas;
	PickingRect pickingRect;

	vector<Render2D*> TypeUI;

	vector<Transform*> transforms;

	Vector3 pickingPos;

	int selectType;

	//��ŷ��ġ �ε����� ����ϱ� ���� ��
	int pickX, pickZ;

	BoxWall* wall;

	static TerrainEditor* instance;

	TerrainEditor(UINT width, UINT height);
	~TerrainEditor();
public:
	static void Create() { instance = new TerrainEditor(mapWidth, mapHeight); }
	static TerrainEditor* Get() { return instance; }
	static void Delete() { delete instance; }

	void Update();
	void Render();
	void PostRender();

	Vector2 GetSize() { return Vector2(width, height); }
	void SetCastle();

private:
	void CreateData();
	void CreateNormal();
	void CreateTangent();

	void MakeRoad();
	void SelectRect(RectRandState changeState, int which);

	void SelectMapToType();

	Transform* AddTransform();
	void UpdateTransforms();

	void InitColor();

	void Save();
	void Load();

};