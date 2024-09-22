#include "Framework.h"

Castle::Castle()
	:GameModel("Castle", L"PropObject")
{
	position = Vector3(mapWidth / 2, 0.0f, mapHeight / 2 + 0.2f);
	scale = Vector3(0.014f, 0.012f, 0.011f);

	Vector2 pos = Vector2(WIN_WIDTH * 0.94f, WIN_HEIGHT * 0.985f);
	Vector2 scale = Vector2(WIN_WIDTH * 0.12f, WIN_HEIGHT * 0.03f);

	lifeCount = 10;

	lifeUI = USER->PushUI(pos, scale);
}

Castle::~Castle()
{
}

void Castle::Update()
{
	
	EnemyGoal();
	lifeUI->SetText(L"³²Àº ¸ñ¼û: " + to_wstring(lifeCount));

	__super::Update();
}

void Castle::Render()
{
	if (lifeCount <= 0)
		return;

	__super::Render();
}

void Castle::lifeTest()
{
	ImGui::Begin("lifeText");

	ImGui::SliderInt("lifeCount", &lifeCount, -100, 100);

	ImGui::End();
}

void Castle::EnemyGoal()
{
	Vector3 min = Vector3(mapWidth / 2 - 1.0f, 0.0f, mapHeight / 2 - 1.0f);
	Vector3 max = Vector3(mapWidth / 2 + 1.0f, 0.0f, mapHeight / 2 + 1.0f);

	for (auto iter = CHARACTER->GetEnemys()->begin();
			  iter != CHARACTER->GetEnemys()->end(); iter++)
	{
		for (UINT i=0; i<iter->second->GetDataSize(); i++)
		{
			if (GameMath::IntersectRect(min, max, iter->second->GetPos(i)))
			{
				iter->second->Dead(i);

				if(lifeCount > 0)
					lifeCount--;
			}
		}
	}
}
