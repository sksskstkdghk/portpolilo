#include "Framework.h"

Character::Character()
	: AModelAnimator(Shader::Add(L"ModelAnimeInstancing")), dataCursor(0)
{
	
}

Character::~Character()
{
	
}

void Character::ParentUpdate()
{
	InstanceRenderDataSet();

	//position.y = terrain->GetPosY(position);
	__super::Update();
}

void Character::InstanceRenderDataSet()
{
	//화면에 그릴 개수
	count = 0;

	for (UINT i = 0; i < charData.size(); i++)
	{
		if (!charData[i].isRender)
			continue;

		Move(i);
		Rotate(i);

		if (charData[i].curHP <= 0)
		{
			Dead(i);
			continue;
		}

		Animation(i);

		//간단하게 객체별 위치로 계산
		if (CAMERA->GetCulling()->PointCulling(&transforms[i]->position))
		{
			transforms[i]->UpdateWorld();

			//범위 안에 있다면 인스턴스 버퍼 내용 복사
			memcpy(&worlds[count], &transforms[i]->GetTransposeWorld(), sizeof(Matrix));
			//상수버퍼 복사
			memcpy(&frameBuffer->data.tweenDesc[count], frameDatas[i], sizeof(TweenDesc));

			count++;

		}
	}

	//버퍼 업데이트
	instanceBuffer->UpdateBuffer(worlds, count);
}

void Character::ParentRender()
{
	__super::Render();
}

void Character::ParentPostRender()
{

}

void Character::Attack(UINT instanceID)
{
	C_ManagerData target;

	if (charDiscrim == CHAR_FRIENDLY)
	{
		target = CHARACTER->FindCloseEnemy(transforms[instanceID]->position);

		charData[instanceID].destPos = CHARACTER->FindNameEnemy(target.name)->GetPos(target.instanceID);

		ReadyToFight(instanceID, target);
	}
	else if (charDiscrim == CHAR_ENEMY)
	{
		target = CHARACTER->FindCloseFriendly(transforms[instanceID]->position);

		charData[instanceID].destPos = CHARACTER->FindNameFriendly(target.name)->GetPos(target.instanceID);

		ReadyToFight(instanceID, target);
	}
}

void Character::SetMinCursor()
{
	for (UINT i = 0; i < charData.size(); i++)
	{
		if (!charData[i].isRender)
		{
			dataCursor = charData[i].instanceID;
			return;
		}
	}

	dataCursor = transforms.size();
}

void Character::Move(UINT instanceID)
{
	AnimState* ani = &charData[instanceID].animState;

	if (*ani == ATTACK)
	{
		Attack(instanceID);
		return;
	}

	MovePath(instanceID);

	if (D3DXVec3Length(&charData[instanceID].direction) > 0.1f || !charData[instanceID].path.empty())
	{
		transforms[instanceID]->position += charData[instanceID].velocity * moveSpeed * DELTA;

		if (*ani != RUN)
		{
			*ani = RUN;
			PlayClip(instanceID, *ani);
		}
	}
	else
	{
		*ani = IDLE;
		PlayClip(instanceID, *ani);
	}
}

void Character::MovePath(UINT instanceID)
{
	if (charData[instanceID].path.empty())
		return;

	Vector3 dest = charData[instanceID].path.back();

	charData[instanceID].direction = dest - transforms[instanceID]->position;
	charData[instanceID].direction.y = 0.0f;

	D3DXVec3Normalize(&charData[instanceID].velocity, &charData[instanceID].direction);

	if (D3DXVec3Length(&charData[instanceID].direction) < 0.1f)
		charData[instanceID].path.pop_back();
}

void Character::Rotate(UINT instanceID)
{
	Vector3 v = charData[instanceID].velocity;
	
	transforms[instanceID]->rotation.y = atan2(-v.x, -v.z);
}

void Character::Dead(UINT instanceID)
{
	charData[instanceID].isRender = false;
	transforms[instanceID]->position = Vector3(1000, 1000, 1000);
}

void Character::ReadyToFight(UINT instanceID, C_ManagerData target)
{
	Vector3* velocity = &charData[instanceID].velocity;
	Vector3* destPos = &charData[instanceID].destPos;

	*velocity = MakeDirection(transforms[instanceID]->position, *destPos);
	float distance = Distance(transforms[instanceID]->position, *destPos);

	if (distance > range)
	{
		transforms[instanceID]->position += *velocity * moveSpeed * DELTA;
		PlayClip(instanceID, RUN);
	}
	else
	{
		PlayClip(instanceID, (UINT)charData[instanceID].animState);

		if (GetCurFrame(instanceID) <= 0 && charData[instanceID].isAttack)
		{
			if(charDiscrim == CHAR_FRIENDLY)
				CHARACTER->FindNameEnemy(target.name)->Demage(target.instanceID, charData[instanceID].attackDeal);
			else if (charDiscrim == CHAR_ENEMY)
				CHARACTER->FindNameFriendly(target.name)->Demage(target.instanceID, charData[instanceID].attackDeal);

			charData[instanceID].isAttack = false;
		}

		if (GetCurFrame(instanceID) > 0)
			charData[instanceID].isAttack = true;
	}
}

//friendly진지 내 캐릭터를 전부 죽인 후 경로 탐색
void Character::EnemyPath(UINT instanceID)
{
	Vector3 pos = transforms[instanceID]->position;
	Vector3 destPosition;
	
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			UINT index = mapWidth / 2 + (mapHeight / 2 - i) * mapWidth - j;
			Vector3 check = ASTAR->GetNodes().at(index)->GetPos();
	
			if (Distance(pos, destPosition) > Distance(pos, check))
				destPosition = check;
		}
	}

	ASTAR->Reset();

	vector<Vector3> path;

	/*Ray ray;
	ray.position = pos;
	D3DXVec3Normalize(&ray.direction, &(destPosition - pos));*/

	int startIndex = ASTAR->FindCloseNode(pos);
	int endIndex = ASTAR->FindCloseNode(destPosition);
	//float distance = Distance(pos, destPosition);

	path = ASTAR->FindPath(startIndex, endIndex);	//길을 찾음
	if (path.size() <= 0)	//길이 없다면 리턴
		return;

	charData[instanceID].destPos = destPosition;

	/*if (ASTAR->isCollisionObstalce(ray, distance))
	{
		ASTAR->MakeDirectPath(pos, destPosition, path);

		path.insert(path.begin(), destPosition);

		int pathSize = path.size();

		while (path.size() > 2)
		{
			vector<Vector3> tempPath;

			for (int i = 1; i < path.size() - 1; i++)
				tempPath.push_back(path.at(i));

			Vector3 start = path.back();
			Vector3 end = path.front();

			ASTAR->MakeDirectPath(start, end, tempPath);

			path.clear();
			path.push_back(end);

			for (int i = 0; i < tempPath.size(); i++)
				path.push_back(tempPath[i]);

			path.push_back(start);

			if (pathSize == path.size())
				break;
			else
				pathSize = path.size();
		}

		path.insert(path.begin(), destPosition);
	}
	else
	{
		path.clear();
		path.shrink_to_fit();

		path.push_back(destPosition);
	}*/

	charData[instanceID].path = path;
	ASTAR->Reset();
}

void Character::Exporter(string name)
{
	AssimpReader* reader = new AssimpReader("FbxData/Models/" + name + ".fbx");
	reader->ExporterMaterial(name + "/" + name);
	reader->ExporterMesh(name + "/" + name);
	delete reader;


	reader = new AssimpReader("FbxData/Animations/" + name + "/Idle.fbx");
	reader->ExporterAnimClip(0, name + "/Idle");
	delete reader;
	reader = new AssimpReader("FbxData/Animations/" + name + "/Run.fbx");
	reader->ExporterAnimClip(0, name + "/Run");
	delete reader;
	reader = new AssimpReader("FbxData/Animations/" + name + "/Attack.fbx");
	reader->ExporterAnimClip(0, name + "/Attack");
	delete reader;
}