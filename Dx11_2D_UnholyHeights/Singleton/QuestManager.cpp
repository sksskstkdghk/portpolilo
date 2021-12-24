#include "stdafx.h"
#include "QuestManager.h"

Quest * QuestManager::AddQuest(int endWave, int questDif, wstring qName, wstring qInfo, string loadName, int gold)
{
	Quest* quest = GetQuest(loadName);

	//추가하려는 퀘스트가 이미 존재한다면 새로만들지 않고 리턴
	if (quest) return quest;

	quest = new Quest(endWave, questDif, qName, qInfo, loadName, gold);
	questmap.insert(make_pair(loadName, quest));

	return quest;
}

Quest * QuestManager::GetOrderQuest()
{
	questIter iter = questmap.begin();

	for(iter; iter != questmap.end();)
	{
		if (iter->second->GetOrder())
			return iter->second;

		iter++;
	}

	return nullptr;
}

Quest * QuestManager::GetQuest(string qName)
{
	questIter iter = questmap.find(qName.c_str());

	if (iter != questmap.end())
	{
		return iter->second;
	}

	return nullptr;
}

void QuestManager::DeleteAll()
{
	questIter iter = questmap.begin();

	for (iter; iter != questmap.end();)
	{
		if (iter->second != nullptr)
		{
			iter->second->Release();
			iter = questmap.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

//게시판에 표시될 5개 퀘스트
void QuestManager::RenderFive(int begin)
{
	questIter iter = questmap.begin();

	for (int i = 0; i < begin; i++)
		iter++;

	for (int i = 0; i < 5; iter++)
	{
		if (iter == questmap.end())
			break;

		if (iter->second->GetClear() != QUEST_CLEAR)
		{
			iter->second->Render(D3DXVECTOR2(409.6f, 480.0f - (i * 70.0f)));
			i++;
		}
	}
}
