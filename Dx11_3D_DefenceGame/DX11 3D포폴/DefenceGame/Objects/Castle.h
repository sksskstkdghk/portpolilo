#pragma once

class Castle : public GameModel
{
private:
	int lifeCount;
	Render2D* lifeUI;

public:
	Castle();
	~Castle();

	void Update();
	void Render();
	void lifeTest();

private:
	void EnemyGoal();

};