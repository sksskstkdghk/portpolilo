#include "Framework.h"

Monster01::Monster01()
	:Character()
{
	//Exporter("Monster01");

	name = "monster01";

	ReadMaterial(name + ".mat", name + "/");
	ReadMesh(name + ".mesh", name + "/");
	ReadClip("Idle.clip", name + "/");
	ReadClip("Run.clip", name + "/");
	ReadClip("Attack.clip", name + "/");

	charDiscrim = CHAR_ENEMY;

	moveSpeed = 1.0f;

	range = 0.5f;
}

void Monster01::Update()
{
	for (UINT i = 0; i < charData.size(); i++)
	{
		if (!charData[i].isRender ||
			Distance(transforms[i]->position, prevPos[i]) < 0.5f)
			continue;

		prevPos[i] = transforms[i]->position;
		CheckCrossRoad(i);
	}

	ParentUpdate();
}

void Monster01::Render()
{
	ParentRender();
}

void Monster01::PostRender()
{
	ParentPostRender();

	int a = charData.size();
	int cursor = (int)dataCursor;

	ImGui::Text("MonsterCount : %d", a);
	//ImGui::Text("mCursor : %d", cursor);

	/*if (charData.size() > 0)
	{
		int b = charData[0].curHP;
		ImGui::Text("monHP", &b, -1000, 1000);
	}*/
}

void Monster01::CheckCrossRoad(UINT index)
{
	Vector3 temp = transforms[index]->position;
	int nodeIndex = (UINT)temp.x + (UINT)temp.z * mapWidth;
	Node* check = ASTAR->GetNode(nodeIndex);

	vector<Vector3> path;
	path.clear();

	for (EdgeInfo* iter : check->GetEdge())
	{
		if (ASTAR->GetNode(iter->index)->GetState() == NONE ||
			ASTAR->GetNode(iter->index)->GetState() == FRIENDLY)
		{
			path = ASTAR->FindCloseFBasePath(iter->index);

			break;
		}
	}

	//Friendly진지 내 캐릭터가 있고 갈 수 있는 경우
	if (path.size() > 0)
	{
		if (charData[index].path[0] == path[0])
			return;
	}
	//Friendly진지 내 캐릭터가 없거나 갈 수 없거나 이미 캐릭터를 전부 죽인 경우
	else
	{
		temp = charData[index].path[0];
		nodeIndex = (UINT)temp.x + (UINT)temp.z * mapWidth;
		check = ASTAR->GetNode(nodeIndex);

		if (check->GetState() == CASTLE)
			return;

		EnemyPath(index);
		SetIDLE(index);
		return;
	}

	charData[index].destPos = path[0];
	charData[index].path.clear();
	charData[index].path = path;
}

C_ManagerData Monster01::Init(UINT lvl, Vector3 pos, vector<Vector3> path)
{
	C_ManagerData returnData;

	SetMinCursor();

	returnData.name = name;
	returnData.instanceID = dataCursor;

	if (charData.size() == dataCursor)
	{
		CharData data;

		Transform* t = AddTransform();
		data.instanceID = dataCursor;
		data.maxHP = 100 * lvl;
		data.curHP = data.maxHP;
		data.attackDeal = 10 * lvl;
		data.defence = 1 * lvl;
		data.animState = IDLE;
		data.isRender = true;
		data.path = path;

		t->position = Vector3(pos.x, 0.0f, pos.z);
		prevPos.push_back(t->position);
		t->scale = Vector3(0.005f, 0.005f, 0.005f);

		charData.push_back(data);

		PlayClip(dataCursor, (UINT)IDLE);

		dataCursor++;
	}
	else
	{
		charData[dataCursor].maxHP = 100 * lvl;
		charData[dataCursor].curHP = charData[dataCursor].maxHP;
		charData[dataCursor].attackDeal = 10 * lvl;
		charData[dataCursor].defence = 1 * lvl;
		charData[dataCursor].animState = IDLE;
		charData[dataCursor].isRender = true;
		charData[dataCursor].isAttack = true;

		charData[dataCursor].path.clear();
		charData[dataCursor].path.shrink_to_fit();
		charData[dataCursor].path = path;

		transforms[dataCursor]->position = Vector3(pos.x, 0.0f, pos.z);
		prevPos[dataCursor] = Vector3(pos.x, 0.0f, pos.z);
		transforms[dataCursor]->scale = Vector3(0.005f, 0.005f, 0.005f);

		PlayClip(dataCursor, (UINT)IDLE);
	}

	return returnData;
}