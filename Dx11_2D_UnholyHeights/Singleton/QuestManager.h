#pragma once

class QuestManager : public SingletonBase<QuestManager>
{
private:
	typedef map<string, Quest*>				questMap;//������ ���� �̹��� ���
	typedef map<string, Quest*>::iterator	questIter;//������ ���� �̹��� ����� �ݺ���

	questMap questmap;

public:
	QuestManager() {}
	~QuestManager() {}

	Quest* AddQuest(int endWave, int questDif, wstring qName, wstring qInfo, string loadName, int gold);
	Quest* GetOrderQuest();
	Quest* GetQuest(string qName);

	void DeleteAll();
	void RenderFive(int begin);
};

