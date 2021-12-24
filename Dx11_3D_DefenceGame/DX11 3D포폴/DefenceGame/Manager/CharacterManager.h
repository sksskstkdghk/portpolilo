#pragma once

class CharacterManager
{
private:
	typedef map<string, Character*> friendlySTL;
	typedef map<string, Character*> enemysSTL;

	friendlySTL friendly;
	enemysSTL enemys;

	static CharacterManager* instance;

	CharacterManager();
	~CharacterManager();

public:
	static void Create() { instance = new CharacterManager(); }
	static CharacterManager* Get() { return instance; }
	static void Delete() { delete instance; }

	C_ManagerData AddFriendly(string name, UINT lvl, Vector3 pos, vector<Vector3> path);
	C_ManagerData AddEnemy(string name, UINT lvl, Vector3 pos, vector<Vector3> path);

	C_ManagerData FindCloseFriendly(Vector3 pos);
	C_ManagerData FindCloseEnemy(Vector3 pos);

	Character* FindNameFriendly(string name);
	Character* FindNameEnemy(string name);

	map<string, Character*>* GetEnemys() { return &enemys; }

	void Update();
	void Render();
	void PostRender();
};
