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

	vector<Vector3> omnidirectional;	//���� Ȯ��

	Vector3 min, max;		//�̷� ���� ��
	Vector3 start, dest;	//����, ������ ��� �˻��� ����

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

	//�ش� �Լ��� ȣ���Ͽ� �̷θ� ����
	void CreateMaze(Vector3 originalStartPos);

	Vector3 GetMin() { return min; }
	Vector3 GetMax() { return max; }

	//�̷λ� ���� �ٽú����� �ٲ�
	void Reset();

private:	//Ŭ������������ ���̴� �Լ��� ����
	MazeAlgorithm();
	~MazeAlgorithm();

	//4���� �� ���� ����� �����ġ�� ã�� ����
	void fourDirCloseDistancePos(Vector3 startPos, Vector3 destPos, Vector3* pOut, float destDis, int range = 1);
	//���� �̷� ���� �Լ�
	void MakeMazePath();

	//�̷� ����� �� ����, ������ ��ġ�� �̷� ũ�� ����
	void SetData(Vector3 originalStartPos);
	//�̷� ������ ������ ����
	void MakeBlock(Vector3 currentPos, Vector3 vec);
};

