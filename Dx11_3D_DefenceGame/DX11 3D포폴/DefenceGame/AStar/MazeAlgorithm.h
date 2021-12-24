#pragma once
#include <queue>

class MazeAlgorithm
{
private:
	struct ReturnNodeData
	{
		int index;
		USHORT state;

		ReturnNodeData(int index, USHORT state)
			:index(index), state(state)
		{}
	};

	vector<Vector3> omnidirectional;	//방향 확인

	Vector3 min, max;		//미로 범위 용
	Vector3 start, dest;	//시작, 목적지 노드 검색용 벡터

	Vector3 dir;

	Node* currentNode;

	vector<Node*> nodeList;
	queue<ReturnNodeData> returnNodes;

	static MazeAlgorithm* instance;

	void Init();
public:
	static void Create() { instance = new MazeAlgorithm(); }
	static void Delete() { delete instance; }
	static MazeAlgorithm* Get() { return instance; }

	//해당 함수를 호출하여 미로를 만듬
	void CreateMaze(Vector3 originalStartPos);

	Vector3 GetMin() { return min; }
	Vector3 GetMax() { return max; }

	//미로상 벽을 다시벽으로 바꿈
	void Reset();

private:	//클래스내에서만 쓰이는 함수들 정의
	MazeAlgorithm();
	~MazeAlgorithm();

	//4방향 중 가장 가까운 노드위치를 찾아 저장
	void fourDirCloseDistancePos(Vector3 startPos, Vector3 destPos, Vector3* pOut, float destDis, int range = 1);
	//실제 미로 생성 함수
	void MakeMazePath();

	//미로 만들기 전 시작, 목적지 위치와 미로 크기 세팅
	void SetData(Vector3 originalStartPos);
	//미로 생성중 벽들을 만듬
	void MakeBlock(Vector3 currentPos, Vector3 vec);
};

