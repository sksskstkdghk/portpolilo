#include "Framework.h"
#include "MazeAlgorithm.h"

MazeAlgorithm* MazeAlgorithm::instance = nullptr;

MazeAlgorithm::MazeAlgorithm()
{
	Init();

	omnidirectional.emplace_back(1.0f, 0.0f, 0.0f);		//우
	omnidirectional.emplace_back(0.0f, 0.0f, 1.0f);		//상
	omnidirectional.emplace_back(-1.0f, 0.0f, 0.0f);	//좌
	omnidirectional.emplace_back(0.0f, 0.0f, -1.0f);	//하

	omnidirectional.emplace_back(1.0f, 0.0f, 1.0f);		//우상
	omnidirectional.emplace_back(1.0f, 0.0f, -1.0f);	//우하
	omnidirectional.emplace_back(-1.0f, 0.0f, 1.0f);	//좌상
	omnidirectional.emplace_back(-1.0f, 0.0f, -1.0f);	//좌하

	nodeList = ASTAR->GetNodes();
}

MazeAlgorithm::~MazeAlgorithm()
{

}

void MazeAlgorithm::fourDirCloseDistancePos(Vector3 startPos, Vector3 destPos, Vector3* pOut, float destDis, int range)
{
	for (int i = -range; i < range * 2; i += range * 2)
	{
		//방향 확인
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
	//주변 노드 저장용
	//해당 노드가 0이면 그전 노드로 돌아감
	vector<Node*> obsNodes;

	//현재 노드 위치
	Vector3 currentPos = currentNode->GetPos();

	for(int i =0; i<4; i++)
	{
		//현재 노드에서 방향 검사
		//대각선은 검사x
		Vector3 vec = omnidirectional[i] + currentPos;

		//노드 인덱스
		int index = (int)vec.z * mapWidth + (int)vec.x;

		//미로 크기를 벗어나면 넘어감
		if (min.x > vec.x || min.z > vec.z ||
			max.x < vec.x || max.z < vec.z)
			continue;

		//저장
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

		//방향 지점
		//길이가 1이기 때문에 정규화 필요X
		Vector3 vec = obsNodes[random]->GetPos() - currentPos;

		MakeBlock(currentPos, vec);

		dir = vec;	//방향 설정

		//이동할 노드가 이미 플레이어가 데이터를 변경한 노드라면
		//길이 막히는 걸 방지하기위해 되돌릴 노드에 저장
		if (obsNodes[random]->state != State::OBSTACLE)
			returnNodes.emplace(obsNodes[random]->index, (USHORT)obsNodes[random]->state);

		//현재 노드의 상태를 미로에서 사용할 노드로 바꾸고
		//다음 노드에 부모로 지정 후 현재노드 = 다음노드 로 변경
		currentNode->state = State::MAZE_ROAD;
		obsNodes[random]->via = currentNode->index;
		currentNode = obsNodes[random];

		obsNodes.erase(obsNodes.begin() + random);	//확인된 길 삭제

		MakeMazePath();
	}

	//한 길을 쭉 탐색했다면
	//아직 탐색하지 않은 방향이 나올때까지 뒤로이동
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

	//간혹 목적지가 닫히는 경우 방지
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
	//미로 생성전 초기화
	Init();

	//미로 크기, 시작, 도착 설정
	SetData(originalStartPos);

	MakeMazePath();

	//노드 초기화
	Reset();
}

void MazeAlgorithm::SetData(Vector3 originalStartPos)
{
	//비교용 시작, 목적지 위치
	Vector3 startPos = Vector3(originalStartPos.x, 0.0f, originalStartPos.z);
	Vector3 destPos;

	float destDistance = 0.0f;

	//중앙에 위치한 캐슬 노드들(2x2) 중 가장 가까운 노드를 찾아 저장해둠
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

	//위에서 구한 위치기준으로 상, 하, 좌, 우 중 가장 가까운 곳을 도착지로 설정
	fourDirCloseDistancePos(startPos, destPos, &dest, destDistance);
	destPos = nodeList[(int)dest.z * mapWidth + (int)dest.x]->GetPos();

	//설정한 도착지를 기준으로 시작위치에서 다시 4방향중 가장 가까운 시작지점을 설정
	fourDirCloseDistancePos(destPos, startPos, &start, destDistance, 2);
	startPos = nodeList[(int)start.z * mapWidth + (int)start.x]->GetPos();

	//범위 확인 및 비교를 위한 최소, 최대 구해서 vector3형태로 저장
	min = Vector3((startPos.x < destPos.x) ? startPos.x : destPos.x,
				   0.0f,
				  (startPos.z < destPos.z) ? startPos.z : destPos.z);

	max = Vector3((startPos.x > destPos.x) ? startPos.x : destPos.x,
				  0.0f,
				  (startPos.z > destPos.z) ? startPos.z : destPos.z);

	currentNode = nodeList[(int)start.z * mapWidth + (int)start.x];

	//미로를 만드는 크기가 너무 작을경우 수정(가로 또는 세로 둘중 하나가 5칸 보다 작을경우)
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
	//2칸이동된 방향
	//해당 부분이 막혔는지 확인해서 따로 처리해야 함
	Vector3 nextFrontPos = currentPos + (vec * 2);

	Vector3 zero = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 dirMinusvec = dir - vec;

	int size = 0;

	//전, 현 방향에 따라 벽이될 노드를 8방향이나 4방향으로 검사
	if (dir != zero && Distance(zero, dirMinusvec) > 0.5f)
		size = omnidirectional.size();
	else
		size = 4;

	for (int i = 0; i < size; i++)
	{
		if (vec == omnidirectional[i] || -vec == omnidirectional[i])
			continue;

		//현재노드에서 미로상 벽으로 바뀌는 주변 노드 방향
		Vector3 sideChange = currentPos + omnidirectional[i];

		//범위 벗어나면 넘어감
		if (sideChange.x < min.x || sideChange.z < min.z ||
			sideChange.x > max.x || sideChange.z > max.z)
			continue;

		//벽으로 바뀌는 노드
		Node* temp = nodeList.at((int)sideChange.z * mapWidth + (int)sideChange.x);

		if ((int)temp->state >= (int)State::MAZE_ROAD &&
			(int)temp->state <= (int)State::MAZE_OBSTACLE)
			continue;

		if (i >= 4)
		{
			//노드 리스트에서 사용
			int index = (int)nextFrontPos.z * mapWidth + (int)nextFrontPos.x;

			//미로를 만드는 맵 밖이거나 2칸이동한 칸이 막힌 벽이라면
			if (nodeList.at(index)->state == State::MAZE_OBSTACLE ||
				nextFrontPos.x < min.x || nextFrontPos.z < min.z || nextFrontPos.x > max.x || nextFrontPos.z > max.z)
			{
				//이동하는 방향에 따라서 벽을 뚫어둠
				//안하면 아예 확인을 안하는 미로상 노드가 생겨버림
				if (sideChange == currentPos + vec + Vector3(0.0f, 0.0f, 1.0f) ||
					sideChange == currentPos + vec + Vector3(0.0f, 0.0f, -1.0f) ||
					sideChange == currentPos + vec + Vector3(-1.0f, 0.0f, 0.0f) ||
					sideChange == currentPos + vec + Vector3(1.0f, 0.0f, 0.0f))
					continue;
			}
		}

		//다 거치면 미로상 벽이됨
		returnNodes.emplace(temp->index, (USHORT)temp->state);
		temp->state = State::MAZE_OBSTACLE;
	}
}
