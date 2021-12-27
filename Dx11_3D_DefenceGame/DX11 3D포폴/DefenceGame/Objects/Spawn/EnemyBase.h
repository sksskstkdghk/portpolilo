#pragma once
#include <functional>

class EnemyBase : public SpawnBase
{
private:
	float time;

	//BoxCollision* collision;
	vector<Vector3> path;

public:
	EnemyBase(Vector3 pos);
	~EnemyBase();

	// SpawnBase을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void CreateCharacter() override;

	vector<Vector3> GetPath() { return path; }

private:
	void CreateEnemy();

	void MakeCastleRoad();
	void EraseSummonData();

	void FunctionalTest01();
};