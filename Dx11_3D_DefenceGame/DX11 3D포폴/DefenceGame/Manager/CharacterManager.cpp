#include "Framework.h"
#include "CharacterManager.h"

CharacterManager* CharacterManager::instance = nullptr;

CharacterManager::CharacterManager()
{
	friendly.insert(make_pair("pikachu", new Pikachu()));

	enemys.insert(make_pair("monster01", new Monster01()));
}

CharacterManager::~CharacterManager()
{
	for (auto it = friendly.begin(); it != friendly.end(); it++)
		delete it->second;

	for (auto it = enemys.begin(); it != enemys.end(); it++)
		delete it->second;

	friendly.clear();
	enemys.clear();
}

C_ManagerData CharacterManager::AddFriendly(string name, UINT lvl, Vector3 pos, vector<Vector3> path)
{
	return friendly[name]->Init(lvl, pos, path);
}

C_ManagerData CharacterManager::AddEnemy(string name, UINT lvl, Vector3 pos, vector<Vector3> path)
{
	return enemys[name]->Init(lvl, pos, path);
}

C_ManagerData CharacterManager::FindCloseFriendly(Vector3 pos)
{
	C_ManagerData closeData;
	Vector3 closePos;

	for (auto it = friendly.begin(); it != friendly.end(); it++)
	{
		if (it == friendly.begin())
		{
			closePos = it->second->GetPos(0);
			closeData.name = it->second->GetName();
			closeData.instanceID = 0;
		}

		for (int i = 0; i < it->second->GetDataSize(); i++)
		{
			if (D3DXVec3Length(&(closePos - pos)) > D3DXVec3Length(&(it->second->GetPos(i) - pos)))
			{
				closePos = it->second->GetPos(i);
				closeData.name = it->second->GetName();
				closeData.instanceID = i;
			}
		}
	}

	return closeData;
}

C_ManagerData CharacterManager::FindCloseEnemy(Vector3 pos)
{
	C_ManagerData closeData;
	Vector3 closePos = Vector3(1000, 1000, 1000);

	for (auto it = enemys.begin(); it != enemys.end(); it++)
	{
		for (int i = 0; i < it->second->GetDataSize(); i++)
		{
			if (!it->second->IsAlive(i))
				continue;

			if (D3DXVec3Length(&(closePos - pos)) > D3DXVec3Length(&(it->second->GetPos(i) - pos)))
			{
				closePos = it->second->GetPos(i);
				closeData.name = it->second->GetName();
				closeData.instanceID = i;
			}
		}
	}

	return closeData;
}

Character* CharacterManager::FindNameFriendly(string name)
{
	if (friendly.count(name) == 0)
		return nullptr;

	return friendly.find(name)->second;
}

Character* CharacterManager::FindNameEnemy(string name)
{
	if (enemys.count(name) == 0)
		return nullptr;

	return enemys.find(name)->second;
}

void CharacterManager::Update()
{
	for (auto it = friendly.begin(); it != friendly.end(); it++)
		it->second->Update();

	for (auto it = enemys.begin(); it != enemys.end(); it++)
		it->second->Update();
}

void CharacterManager::Render()
{
	for (auto it = friendly.begin(); it != friendly.end(); it++)
		it->second->Render();

	for (auto it = enemys.begin(); it != enemys.end(); it++)
		it->second->Render();
}

void CharacterManager::PostRender()
{
	for (auto it = friendly.begin(); it != friendly.end(); it++)
		it->second->PostRender();

	for (auto it = enemys.begin(); it != enemys.end(); it++)
		it->second->PostRender();
}
