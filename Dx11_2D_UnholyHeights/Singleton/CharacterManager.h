#pragma once
#include "SingletonBase.h"

class CharacterManager :public SingletonBase<CharacterManager>
{
private:
	typedef map<string, Character*>				monMap;//������ ���� �̹��� ���
	typedef map<string, Character*>::iterator	monIter;//������ ���� �̹��� ����� �ݺ���

	typedef map<string, Character*>				braveMap;//������ ���� �̹��� ���
	typedef map<string, Character*>::iterator	braveIter;//������ ���� �̹��� ����� �ݺ���

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

