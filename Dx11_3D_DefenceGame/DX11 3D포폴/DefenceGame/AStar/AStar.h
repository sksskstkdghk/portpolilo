#pragma once

class TerrainEditor;

class AStar
{
private:
	UINT width, height;

	vector<Node*> nodes;
	//vector<int> openNodes;
	Heap* heap;

	BoxCollision* obstacles;

	SphereCollision* sphere;	//����׿� ��

	static AStar* instance;

	AStar(UINT width = 20, UINT height = 20);
	~AStar();

public:
	static void Create(UINT width, UINT height) { instance = new AStar(width, height); }
	static void Delete() { delete instance; }
	static AStar* Get(){ return instance; }

	void SetUp(TerrainEditor* terrain);
	void Update();
	void Render();

	vector<Vector3> FindPath(int start, int end);	//�Ϲ����� �� ã�� �Լ�
	vector<Vector3> FindCloseFBasePath(int start);	//���� ����� FBase�� ��θ� ã�� �Լ�
	int FindCloseNode(Vector3 pos, bool cantObs = true);
	void MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path);

	bool isWorkObstalce(Vector3 pickingPos);
	bool isCollisionObstalce(Ray ray, float destDistance);

	void Reset();
	//void ReturnObstacle();

public:	//get, set ����
	vector<Node*> GetNodes() { return nodes; }
	Node* GetNode(UINT index) { return nodes.at(index); }

private:
	float GetManhattanDistance(int curIndex, int end);
	float GetDistance(int curIndex, int end);

	void Extend(int center, int end);	//�����忡�� �ֺ� ��� �� ����
	void CloseFBaseExtend(int center, int end);
	int GetMinNode();
};