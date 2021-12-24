#pragma once

class SpawnManager
{
private:
	typedef vector<FriendlyBase*> Friendly;
	typedef vector<EnemyBase*>	  Enemy;

	Friendly FBase;
	Enemy	 EBase;

	static SpawnManager* instance;

	SpawnManager() {}
	~SpawnManager();

public:	//�̱��� ����
	static void Create() { instance = new SpawnManager(); }
	static SpawnManager* Get() { return instance; }
	static void Delete() { delete instance; }

public:	//����� �Լ���
	void AddWhichBase(Vector3 pos, int which);

	SpawnBase* GetFBase(UINT index);
	SpawnBase* GetEBase(UINT index);

	int GetFBaseSize() { return FBase.size(); }
	int GetEBaseSize() { return EBase.size(); }
	
	void Update();
	void Render();

private: //���� �Լ�
	void AddFriendlyBase(Vector3 pos);
	void AddEnemyBase(Vector3 pos);
};