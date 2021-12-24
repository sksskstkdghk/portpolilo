#pragma once
#include "SingletonBase.h"

class CharacterManager :public SingletonBase<CharacterManager>
{
private:
	typedef map<string, Character*>				monMap;//맵으로 만든 이미지 목록
	typedef map<string, Character*>::iterator	monIter;//맵으로 만든 이미지 목록의 반복자

	typedef map<string, Character*>				braveMap;//맵으로 만든 이미지 목록
	typedef map<string, Character*>::iterator	braveIter;//맵으로 만든 이미지 목록의 반복자

	monMap monsterMap;
	braveMap humanMap;

	int nameNum;
	int braveNum;

public:
	CharacterManager();
	~CharacterManager();

	Character* AddMonster(string name, int gender, int tribe);
	Character* AddBrave(string name, int gender, int tribe);

	Character* GetMonster(string name);
	Character* GetBrave(string name);

	Character* GetStateMonster(int st);

	vector<Character*> GetSameStateMonsters(int st);
	vector<Character*> GetAllBrave();

	bool DeleteMonster(string name);

	void DeleteBrave();
	void DeleteAll();
	void DeleteDeadCTR();
	void RenderAll();
	void UpdateAll();
};

