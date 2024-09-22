#include "Framework.h"

Pikachu::Pikachu()
	: Character()
{
	name = "pikachu";

	ReadMaterial(name + ".mat", name + "/");
	ReadMesh(name + ".mesh", name + "/");
	ReadClip("Idle.clip", name + "/");
	ReadClip("Run.clip", name + "/");
	ReadClip("Attack.clip", name + "/");

	charDiscrim = CHAR_FRIENDLY;

	moveSpeed = 10.0f;

	range = 0.5f;
}

void Pikachu::Update()
{
	ParentUpdate();
}

void Pikachu::Render()
{
	ParentRender();
}

void Pikachu::PostRender()
{
	ParentPostRender();

	int a = charData.size();

	ImGui::Text("pikaCount : %d", a);
	

	/*if (charData.size() > 0)
	{
		float b = GetCurFrame(0);
		int c = charData[0].curHP;

		ImGui::SliderFloat("pikaCurFrame", &b, -1000, 1000);
		ImGui::SliderInt("pikaHP", &c, -1000, 1000);
	}*/
}

C_ManagerData Pikachu::Init(UINT lvl, Vector3 pos, vector<Vector3> path)
{
	C_ManagerData returnData;

	SetMinCursor();

	if (transforms.size() == dataCursor)
	{
		CharData data;

		Transform* t = AddTransform();
		data.instanceID = dataCursor;
		data.maxHP = 100 * lvl;
		data.curHP = data.maxHP;
		data.attackDeal = 10 * lvl;
		data.defence = 1 * lvl;
		data.animState = IDLE;
		data.isRender = 1;

		t->position = Vector3(pos.x, 0.0f, pos.z);
		t->scale = Vector3(0.005f, 0.005f, 0.005f);

		charData.push_back(data);

		PlayClip(dataCursor, (UINT)IDLE);

		returnData.name = name;
		returnData.instanceID = dataCursor;

		dataCursor++;
	}
	else
	{
		charData[dataCursor].maxHP = 100 * lvl;
		charData[dataCursor].curHP = charData[dataCursor].maxHP;
		charData[dataCursor].attackDeal = 10 * lvl;
		charData[dataCursor].defence = 1 * lvl;
		charData[dataCursor].animState = IDLE;
		charData[dataCursor].isRender = 1;
		charData[dataCursor].isAttack = true;
		charData[dataCursor].path.clear();

		transforms[dataCursor]->position = Vector3(pos.x, 0.0f, pos.z);
		transforms[dataCursor]->scale = Vector3(0.005f, 0.005f, 0.005f);

		PlayClip(dataCursor, (UINT)IDLE);

		returnData.name = name;
		returnData.instanceID = dataCursor;
	}

	return returnData;
}
