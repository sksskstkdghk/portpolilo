#include "Framework.h"
#include "MazeAlgorithm.h"

MazeAlgorithm* MazeAlgorithm::instance = nullptr;

MazeAlgorithm::MazeAlgorithm()
{
	Init();

	omnidirectional.emplace_back(1.0f, 0.0f, 0.0f);		//��
	omnidirectional.emplace_back(0.0f, 0.0f, 1.0f);		//��
	omnidirectional.emplace_back(-1.0f, 0.0f, 0.0f);	//��
	omnidirectional.emplace_back(0.0f, 0.0f, -1.0f);	//��

	omnidirectional.emplace_back(1.0f, 0.0f, 1.0f);		//���
	omnidirectional.emplace_back(1.0f, 0.0f, -1.0f);	//����
	omnidirectional.emplace_back(-1.0f, 0.0f, 1.0f);	//�»�
	omnidirectional.emplace_back(-1.0f, 0.0f, -1.0f);	//����

	nodeList = ASTAR->GetNodes();
}

MazeAlgorithm::~MazeAlgorithm()
{

}

void MazeAlgorithm::fourDirCloseDistancePos(Vector3 startPos, Vector3 destPos, Vector3* pOut, float destDis, int range)
{
	for (int i = -range; i < range * 2; i += range * 2)
	{
		//���� Ȯ��
		Vector3 tempX = Vector3(destPos.x + i, 0.0f, destPos.z);
		Vector3 tempZ = Vector3(destPos.x, 0.0f, destPos.z + i);

		float x = D3DXVec3Length(&(startPos - tempX));
		float z = D3DXVec3Length(&(startPos - tempZ));

		if (destDis > x || destDis > z)
			*pOut = (x < z) ? Vector3((int)tempX.x, 0.0f, (int)tempX.z)  : Vector3((int)tempZ.x, 0.0f, (int)tempZ.z);
	}
}

void MazeAlgorithm::MakeMazePath()
{
	//�ֺ� ��� �����
	//�ش� ��尡 0�̸� ���� ���� ���ư�
	vector<Node*> obsNodes;

	//���� ��� ��ġ
	Vector3 currentPos = currentNode->GetPos();

	for(int i =0; i<4; i++)
	{
		//���� ��忡�� ���� �˻�
		//�밢���� �˻�x
		Vector3 vec = omnidirectional[i] + currentPos;

		//��� �ε���
		int index = (int)vec.z * mapWidth + (int)vec.x;

		//�̷� ũ�⸦ ����� �Ѿ
		if (min.x > vec.x || min.z > vec.z ||
			max.x < vec.x || max.z < vec.z)
			continue;

		//����
		obsNodes.emplace_back(nodeList.at(index));
	}

	while (obsNodes.size() > 0)
	{
		int random = Random(0, obsNodes.size());

		if ((int)obsNodes[random]->state >= (int)State::MAZE_ROAD &&
			(int)obsNodes[random]->state <= (int)State::MAZE_OBSTACLE)
		{
			obsNodes.erase(obsNodes.begin() + random);
			continue;
		}

		//���� ����
		//���̰� 1�̱� ������ ����ȭ �ʿ�X
		Vector3 vec = obsNodes[random]->GetPos() - currentPos;

		MakeBlock(currentPos, vec);

		dir = vec;	//���� ����

		//�̵��� ��尡 �̹� �÷��̾ �����͸� ������ �����
		//���� ������ �� �����ϱ����� �ǵ��� ��忡 ����
		if (obsNodes[random]->state != State::OBSTACLE)
			returnNodes.emplace(obsNodes[random]->index, (USHORT)obsNodes[random]->state);

		//���� ����� ���¸� �̷ο��� ����� ���� �ٲٰ�
		//���� ��忡 �θ�� ���� �� ������ = ������� �� ����
		currentNode->state = State::MAZE_ROAD;
		obsNodes[random]->via = currentNode->index;
		currentNode = obsNodes[random];

		obsNodes.erase(obsNodes.begin() + random);	//Ȯ�ε� �� ����

		MakeMazePath();
	}

	//�� ���� �� Ž���ߴٸ�
	//���� Ž������ ���� ������ ���ö����� �ڷ��̵�
	if (currentNode->via != start.z * mapWidth + start.x)
	{
		currentNode->state = State::MAZE_ROAD;
		currentNode = nodeList[currentNode->via];
	}
}

void MazeAlgorithm::Reset()
{
	/*for (int i = min.z; i <= max.z; i++)
	{
		for (int j = min.x; j <= max.x; j++)
		{
			int index = i * mapWidth + j;

			if (nodeList[index]->state == State::MAZE_OBSTACLE)
				nodeList[index]->state = State::OBSTACLE;
			else if (nodeList[index]->state == State::CLOSED ||
				nodeList[index]->state == State::OPEN)
				nodeList[index]->state = State::NONE;
			else if (nodeList[index]->state == State::MONSTER_CLOSED ||
				nodeList[index]->state == State::MONSTER_OPEN)
				nodeList[index]->state = State::MONSTER_ROAD;
		}
	}*/

	while (!returnNodes.empty())
	{
		nodeList[returnNodes.front().index]->state = (State)returnNodes.front().state;
		returnNodes.pop();
	}

	//��Ȥ �������� ������ ��� ����
	nodeList[(int)dest.z * mapWidth + (int)dest.x]->state = State::MAZE_ROAD;
}

void MazeAlgorithm::Init()
{
	min = Vector3(0.0f, 0.0f,0.0f);
	max = Vector3(0.0f, 0.0f, 0.0f);
	start = Vector3(0.0f, 0.0f, 0.0f);
	dest = Vector3(0.0f, 0.0f, 0.0f);

	dir = Vector3(0.0f, 0.0f, 0.0f);

	currentNode = nullptr;
}

void MazeAlgorithm::CreateMaze(Vector3 originalStartPos)
{
	//�̷� ������ �ʱ�ȭ
	Init();

	//�̷� ũ��, ����, ���� ����
	SetData(originalStartPos);

	MakeMazePath();

	//��� �ʱ�ȭ
	Reset();
}

void MazeAlgorithm::SetData(Vector3 originalStartPos)
{
	//�񱳿� ����, ������ ��ġ
	Vector3 startPos = Vector3(originalStartPos.x, 0.0f, originalStartPos.z);
	Vector3 destPos;

	float destDistance = 0.0f;

	//�߾ӿ� ��ġ�� ĳ�� ����(2x2) �� ���� ����� ��带 ã�� �����ص�
	for (UINT z = 0; z < 2; z++)
		for (UINT x = 0; x < 2; x++)
		{
			UINT index = mapWidth / 2 + (mapHeight / 2 - z) * mapWidth - x;
			Vector3 tempPos = nodeList.at(index)->GetPos();

			float temp = D3DXVec3Length(&(startPos - tempPos));

			if (destDistance == 0.0f || temp < destDistance)
			{
				destDistance = temp;
				destPos = tempPos;
			}
		}

	//������ ���� ��ġ�������� ��, ��, ��, �� �� ���� ����� ���� �������� ����
	fourDirCloseDistancePos(startPos, destPos, &dest, destDistance);
	destPos = nodeList[(int)dest.z * mapWidth + (int)dest.x]->GetPos();

	//������ �������� �������� ������ġ���� �ٽ� 4������ ���� ����� ���������� ����
	fourDirCloseDistancePos(destPos, startPos, &start, destDistance, 2);
	startPos = nodeList[(int)start.z * mapWidth + (int)start.x]->GetPos();

	//���� Ȯ�� �� �񱳸� ���� �ּ�, �ִ� ���ؼ� vector3���·� ����
	min = Vector3((startPos.x < destPos.x) ? startPos.x : destPos.x,
				   0.0f,
				  (startPos.z < destPos.z) ? startPos.z : destPos.z);

	max = Vector3((startPos.x > destPos.x) ? startPos.x : destPos.x,
				  0.0f,
				  (startPos.z > destPos.z) ? startPos.z : destPos.z);

	currentNode = nodeList[(int)start.z * mapWidth + (int)start.x];

	//�̷θ� ����� ũ�Ⱑ �ʹ� ������� ����(���� �Ǵ� ���� ���� �ϳ��� 5ĭ ���� �������)
	while (max.x - min.x < 5)
	{
		max.x++;
		min.x--;
	}

	while (max.z - min.z < 5)
	{
		max.z++;
		min.z--;
	}
}

void MazeAlgorithm::MakeBlock(Vector3 currentPos, Vector3 vec)
{
	//2ĭ�̵��� ����
	//�ش� �κ��� �������� Ȯ���ؼ� ���� ó���ؾ� ��
	Vector3 nextFrontPos = currentPos + (vec * 2);

	Vector3 zero = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 dirMinusvec = dir - vec;

	int size = 0;

	//��, �� ���⿡ ���� ���̵� ��带 8�����̳� 4�������� �˻�
	if (dir != zero && Distance(zero, dirMinusvec) > 0.5f)
		size = omnidirectional.size();
	else
		size = 4;

	for (int i = 0; i < size; i++)
	{
		if (vec == omnidirectional[i] || -vec == omnidirectional[i])
			continue;

		//�����忡�� �̷λ� ������ �ٲ�� �ֺ� ��� ����
		Vector3 sideChange = currentPos + omnidirectional[i];

		//���� ����� �Ѿ
		if (sideChange.x < min.x || sideChange.z < min.z ||
			sideChange.x > max.x || sideChange.z > max.z)
			continue;

		//������ �ٲ�� ���
		Node* temp = nodeList.at((int)sideChange.z * mapWidth + (int)sideChange.x);

		if ((int)temp->state >= (int)State::MAZE_ROAD &&
			(int)temp->state <= (int)State::MAZE_OBSTACLE)
			continue;

		if (i >= 4)
		{
			//��� ����Ʈ���� ���
			int index = (int)nextFrontPos.z * mapWidth + (int)nextFrontPos.x;

			//�̷θ� ����� �� ���̰ų� 2ĭ�̵��� ĭ�� ���� ���̶��
			if (nodeList.at(index)->state == State::MAZE_OBSTACLE ||
				nextFrontPos.x < min.x || nextFrontPos.z < min.z || nextFrontPos.x > max.x || nextFrontPos.z > max.z)
			{
				//�̵��ϴ� ���⿡ ���� ���� �վ��
				//���ϸ� �ƿ� Ȯ���� ���ϴ� �̷λ� ��尡 ���ܹ���
				if (sideChange == currentPos + vec + Vector3(0.0f, 0.0f, 1.0f) ||
					sideChange == currentPos + vec + Vector3(0.0f, 0.0f, -1.0f) ||
					sideChange == currentPos + vec + Vector3(-1.0f, 0.0f, 0.0f) ||
					sideChange == currentPos + vec + Vector3(1.0f, 0.0f, 0.0f))
					continue;
			}
		}

		//�� ��ġ�� �̷λ� ���̵�
		returnNodes.emplace(temp->index, (USHORT)temp->state);
		temp->state = State::MAZE_OBSTACLE;
	}
}
