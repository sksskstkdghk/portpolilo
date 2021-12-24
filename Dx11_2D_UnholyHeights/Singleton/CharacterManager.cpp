#include "stdafx.h"
#include "CharacterManager.h"


CharacterManager::CharacterManager()
{
	nameNum = 1;
	braveNum = 1;
}


CharacterManager::~CharacterManager()
{
}

Character * CharacterManager::AddMonster(string name, int gender, int tribe)
{
	string str = name + to_string(nameNum);
	Character* character = GetMonster(str);

	//추가하려는 이미지가 이미 존재한다면 새로만들지 않고 리턴
	if (character) return character;

	//존재하지 않는다면
	character = new Character(str, gender, tribe);
	character->SetState(STATE_FIND_HOUSE);
	//생성된 이미지를 맵으로 만든 목록에 추가한다
	monsterMap.insert(make_pair(str, character));
	nameNum++;
	return character;
}

Character * CharacterManager::AddBrave(string name, int gender, int tribe)
{
	string str = name + to_string(braveNum);
	Character* character = GetBrave(str);

	//추가하려는 이미지가 이미 존재한다면 새로만들지 않고 리턴
	if (character) return character;

	//존재하지 않는다면
	character = new Character(str, gender, tribe);
	character->SetState(STATE_BRAVE);
	//생성된 이미지를 맵으로 만든 목록에 추가한다
	humanMap.insert(make_pair(str, character));
	braveNum++;

	return character;
}

Character * CharacterManager::GetMonster(string strkey)
{
	monIter iter = monsterMap.find(strkey);

	if (iter != monsterMap.end())
	{
		return iter->second;
	}

	return nullptr;
}

Character * CharacterManager::GetBrave(string name)
{
	braveIter iter = humanMap.find(name);

	if (iter != humanMap.end())
	{
		return iter->second;
	}

	return nullptr;
}

Character * CharacterManager::GetStateMonster(int st)
{
	monIter iter = monsterMap.begin();

	for (iter; iter != monsterMap.end();)
	{
		if (iter->second->GetState() == st)
			return iter->second;

		iter++;
	}

	return nullptr;
}

vector<Character*> CharacterManager::GetSameStateMonsters(int st)
{
	vector<Character*> monster;
	monIter iter = monsterMap.begin();

	for (iter; iter != monsterMap.end();)
	{
		if(iter->second->GetState() == st &&
			iter->second->GetCurrentHP() > 0)
			monster.emplace_back(iter->second);

		iter++;
	}

	return monster;
}

vector<Character*> CharacterManager::GetAllBrave()
{
	vector<Character*> brave;
	braveIter iter = humanMap.begin();

	for(iter; iter != humanMap.end();)
	{
		if(iter->second->GetCurrentHP() > 0)
			brave.emplace_back(iter->second);

		iter++;
	}

	return brave;
}

bool CharacterManager::DeleteMonster(string name)
{
	monIter iter = monsterMap.find(name);
	
	if (iter != monsterMap.end())
	{
		SAFE_DELETE(iter->second);

		monsterMap.erase(iter);
		return true;
	}
	return false;
}

void CharacterManager::DeleteBrave()
{
	braveIter iterH = humanMap.begin();
	for (iterH; iterH != humanMap.end();)
	{
		if (iterH->second != nullptr)
		{
			SAFE_DELETE(iterH->second);
			iterH = humanMap.erase(iterH);
		}
		else
		{
			iterH++;
		}
	}

	humanMap.clear();
}

void CharacterManager::DeleteAll()
{
	monIter iter = monsterMap.begin();
	for (iter; iter != monsterMap.end();)
	{
		if (iter->second != nullptr)
		{
			SAFE_DELETE(iter->second);
			iter = monsterMap.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	braveIter iterH = humanMap.begin();
	for (iterH; iterH != humanMap.end();)
	{
		if (iterH->second != nullptr)
		{
			SAFE_DELETE(iterH->second);
			iterH = humanMap.erase(iterH);
		}
		else
		{
			iterH++;
		}
	}

	monsterMap.clear();
	humanMap.clear();
}

void CharacterManager::DeleteDeadCTR()
{
	monIter iter = monsterMap.begin();
	for (iter; iter != monsterMap.end();)
	{
		if (iter->second->GetCurrentHP() <= 0)
		{
			SAFE_DELETE(iter->second);
			iter = monsterMap.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	braveIter iterH = humanMap.begin();
	for (iterH; iterH != humanMap.end();)
	{
		if (iterH->second->GetCurrentHP() <= 0)
		{
			SAFE_DELETE(iterH->second);
			iterH = humanMap.erase(iterH);
		}
		else
		{
			iterH++;
		}
	}
}

void CharacterManager::RenderAll()
{
	monIter iter = monsterMap.begin();
	for (iter; iter != monsterMap.end();)
	{
		if (iter->second != nullptr)
			iter->second->Render();

		iter++;
	}

	braveIter iterH = humanMap.begin();
	for (iterH; iterH != humanMap.end();)
	{
		if (iterH->second != nullptr)
			iterH->second->Render();

		iterH++;
	}
}

void CharacterManager::UpdateAll()
{
	monIter iter = monsterMap.begin();
	for (iter; iter != monsterMap.end();)
	{
		if (iter->second != nullptr)
			iter->second->Update();

		iter++;
	}

	braveIter iterH = humanMap.begin();
	for (iterH; iterH != humanMap.end();)
	{
		if (iterH->second != nullptr)
			iterH->second->Update();

		iterH++;
	}
}
