#include "Framework.h"

SpawnManager* SpawnManager::instance = nullptr;

SpawnManager::~SpawnManager()
{
	for (FriendlyBase* f : FBase)
		delete f;

	for (EnemyBase* e : EBase)
		delete e;

	FBase.clear();
	FBase.shrink_to_fit();

	EBase.clear();
	EBase.shrink_to_fit();
}

void SpawnManager::AddFriendlyBase(Vector3 pos)
{
	Vector3 p = Vector3((UINT)pos.x, 0.01f, (UINT)pos.z);

	FriendlyBase* f = new FriendlyBase();
	f->position = Vector3(p.x + 0.5f, p.y, p.z + 0.5f);

	FBase.push_back(f);
}

void SpawnManager::AddEnemyBase(Vector3 pos)
{
	Vector3 p = Vector3((UINT)pos.x, 0.01f, (UINT)pos.z);

	EnemyBase* e = new EnemyBase(p);

	EBase.push_back(e);
}

void SpawnManager::AddWhichBase(Vector3 pos, int which)
{
	if (which == 2)
		AddFriendlyBase(pos);

	if (which == 3)
		AddEnemyBase(pos);
}

SpawnBase* SpawnManager::GetFBase(UINT index)
{
	return FBase.at(index);
}

SpawnBase* SpawnManager::GetEBase(UINT index)
{
	return EBase.at(index);
}

void SpawnManager::Update()
{
	for (UINT i = 0; i < FBase.size(); i++)
		FBase.at(i)->Update();

	for (UINT i = 0; i < EBase.size(); i++)
		EBase.at(i)->Update();
}

void SpawnManager::Render()
{
	for (UINT i = 0; i < FBase.size(); i++)
		FBase.at(i)->Render();

	for (UINT i = 0; i < EBase.size(); i++)
		EBase.at(i)->Render();
}
