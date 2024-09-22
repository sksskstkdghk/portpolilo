#pragma once

class QuestManager : public SingletonBase<QuestManager>
{
private:
	typedef map<string, Quest*>				questMap;//맵으로 만든 이미지 목록
	typedef map<string, Quest*>::iterator	questIter;//맵으로 만든 이미지 목록의 반복자

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

