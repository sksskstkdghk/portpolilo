#include "Framework.h"

EnemyBase::EnemyBase(Vector3 pos)
	:SpawnBase()
{
	time = 0.0f;

	summonData.name = "monster01";
	summonData.summonTime = 2.0f;

	position = Vector3(pos.x + 0.5f, pos.y, pos.z + 0.5f);

	MakeCastleRoad();
}

EnemyBase::~EnemyBase()
{
	path.clear();
	path.shrink_to_fit();
}

void EnemyBase::Update()
{
	static int isMake = -1;

	/*if (KEYDOWN('P'))
	{
		isMake++;

		switch (isMake)
		{
		case 0:
			MAZE->Reset();
			break;
		case 1:
			FunctionalTest01();
			break;
		}
	}

	if (isMake < 1)
		return;*/

	UpdateWorld();
	CreateCharacter();
}

void EnemyBase::Render()
{
	SetDraw();
}

void EnemyBase::CreateCharacter()
{
	if (time > 0.0f)
	{
		time -= DELTA;
		return;
	}

	if (summonCharacters >= 10)
		return;

	CreateEnemy();

	EraseSummonData();
}

void EnemyBase::CreateEnemy()
{
	if (summonData.name == "monster01")
	{
		useChar.push_back(CHARACTER->AddEnemy("monster01", baseLVL, position, path));

		summonCharacters++;

		time = summonData.summonTime;
	}
}

void EnemyBase::MakeCastleRoad()
{
	//미로 생성
	MAZE->CreateMaze(position);

	FunctionalTest01();
}

void EnemyBase::EraseSummonData()
{
	if (useChar.size() < 0)
		return;

	for (UINT i = 0; i < useChar.size(); i++)
	{
		Character* cursor = nullptr;

		if (CHARACTER->FindNameEnemy(useChar[i].name) != nullptr)
		{
			cursor = CHARACTER->FindNameEnemy(useChar[i].name);

			if (!cursor->IsAlive(useChar[i].instanceID))
			{
				useChar.erase(useChar.begin() + i);
				summonCharacters--;

				continue;
			}
		}
	}
}

void EnemyBase::FunctionalTest01()
{
	Vector3 destPosition = Vector3(1000, 1000, 1000);

	Vector3 min = MAZE->GetMin();
	Vector3 max = MAZE->GetMax();

	//가장 가까운 노드상에 성 위치 찾기
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			UINT index = mapWidth / 2 + (mapHeight / 2 - i) * mapWidth - j;
			Vector3 a = ASTAR->GetNodes().at(index)->GetPos();

			if (Distance(position, destPosition) > Distance(position, a))
				destPosition = a;
		}

	int startIndex = ASTAR->FindCloseNode(position, false);
	int endIndex = ASTAR->FindCloseNode(destPosition);

	path = ASTAR->FindPath(startIndex, endIndex);

	if (path.size() == 0)
		return;

	for (Vector3 iter : path)
		ASTAR->GetNode((int)iter.z * mapWidth + (int)iter.x)->SetState(MONSTER_ROAD);

	//미로 생성후 길을 찾은뒤 남아있는 길을 되돌림
	for (int i = min.z; i <= max.z; i++)
	{
		for (int j = min.x; j <= max.x; j++)
		{
			int index = i * mapWidth + j;

			if (ASTAR->GetNode(index)->GetState() >= State::MAZE_ROAD)
				ASTAR->GetNode(index)->SetState(OBSTACLE);
		}
	}

	ASTAR->Reset();	//노드들 상태 초기화
}

void EnemyBase::FunctionalTest02()
{
}
