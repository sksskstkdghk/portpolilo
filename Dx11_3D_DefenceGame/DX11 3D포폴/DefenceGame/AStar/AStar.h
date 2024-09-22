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

	SphereCollision* sphere;	//디버그용 공

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

	vector<Vector3> FindPath(int start, int end);	//일반적인 길 찾는 함수
	vector<Vector3> FindCloseFBasePath(int start);	//가장 가까운 FBase에 경로를 찾는 함수
	int FindCloseNode(Vector3 pos, bool cantObs = true);
	void MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path);

	bool isWorkObstalce(Vector3 pickingPos);
	bool isCollisionObstalce(Ray ray, float destDistance);

	void Reset();
	//void ReturnObstacle();

public:	//get, set 전용
	vector<Node*> GetNodes() { return nodes; }
	Node* GetNode(UINT index) { return nodes.at(index); }

private:
	float GetManhattanDistance(int curIndex, int end);
	float GetDistance(int curIndex, int end);

	void Extend(int center, int end);	//현재노드에서 주변 노드 값 설정
	void CloseFBaseExtend(int center, int end);
	int GetMinNode();
};