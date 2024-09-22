#pragma once

struct Brave
{
	string key;
	int braveName;

	Brave(string key, int braveName)
	{
		this->key = key;
		this->braveName = braveName;
	}
};

enum IsClear
{
	QUEST_NONE = 0,
	QUEST_PROGRESS, QUEST_CLEAR, QUEST_FAIL
};

class Quest
{
private:
	Button* btn;
	bool isOrder;
	IsClear isClear;
	int wave, endWave;
	float time, soundTime;
	int questDif;
	int questGold;
	string loadName;
	wstring questName, questInfo;
	vector<Brave*> waveBrave;

public:
	Quest(int endWave, int questDif, wstring qName, wstring qInfo, string loadName, int gold);
	~Quest();

	float GetTime() { return time; }
	float GetSoundTime() { return soundTime; }
	int GetGold() { return questGold; }
	int GetQuestDif() { return questDif; }
	int GetCurrentWave() { return wave; }
	int GetendWave() { return endWave; }
	IsClear GetClear() { return isClear; }
	bool GetOrder() { return isOrder; }
	string GetLoadName() { return loadName; }
	wstring GetQuestName() { return questName; }
	wstring GetQuestInfo() { return questInfo; }

	void SetWaveBrave(int wave, Tribe t) { waveBrave.emplace_back(new Brave(to_string(wave) + "wave", (int)t)); }

	void SoundTime();
	void SetOrder(bool order) { isOrder = order; }
	void SetClear(IsClear clear) { isClear = clear; }
	bool QuestOrder() { return btn->IsKeyDown(VK_LBUTTON); }

	void Update();
	void Release();
	void Render(D3DXVECTOR2 pos);
	void Clear();
};

