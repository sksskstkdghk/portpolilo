#include "stdafx.h"
#include "QuestManager.h"

Quest * QuestManager::AddQuest(int endWave, int questDif, wstring qName, wstring qInfo, string loadName, int gold)
{
	Quest* quest = GetQuest(loadName);

	//�߰��Ϸ��� ����Ʈ�� �̹� �����Ѵٸ� ���θ����� �ʰ� ����
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

//�Խ��ǿ� ǥ�õ� 5�� ����Ʈ
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
