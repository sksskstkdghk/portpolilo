#include "stdafx.h"
#include "Quest.h"


Quest::Quest(int endWave, int questDif, wstring qName, wstring qInfo, string loadName, int gold)
{
	isOrder = false;
	isClear = QUEST_NONE;
	questName = qName;
	questInfo = qInfo;
	questGold = gold;
	this->loadName = loadName;
	this->questDif = questDif;
	this->endWave = endWave;

	time = 0.0f;
	soundTime = 0.0f;

	btn = new Button("kanban_header", D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));

	wave = 1;
}


Quest::~Quest()
{
}

void Quest::SoundTime()
{
	if (soundTime > 0.0f)
		soundTime -= g_ETime;

	if (time > 0.0f && isClear != QUEST_PROGRESS)
		time -= g_ETime;

	if (CHARACTERMANAGER->GetAllBrave().size() <= 0)
	{
		if (isOrder && wave > endWave && isClear == QUEST_PROGRESS)
		{
			Clear();
			CHARACTERMANAGER->DeleteDeadCTR();
		}
	}
	else if (isClear == QUEST_PROGRESS &&
		CHARACTERMANAGER->GetAllBrave().at(CHARACTERMANAGER->GetAllBrave().size() - 1)->GetGold() > 0 &&
		CHARACTERMANAGER->GetAllBrave().at(CHARACTERMANAGER->GetAllBrave().size() - 1)->GetPos().x <= -10.0f &&
		CHARACTERMANAGER->GetAllBrave().at(CHARACTERMANAGER->GetAllBrave().size() - 1)->GetCurrentHP() > 0)
	{
		isClear = QUEST_FAIL;
		wave = 1;
		isOrder = false;
		time = 7.0f;

		CHARACTERMANAGER->DeleteDeadCTR();
		CHARACTERMANAGER->DeleteBrave();
	}
}

void Quest::Update()
{
	if (CHARACTERMANAGER->GetAllBrave().size() <= 0)
	{
		if (isOrder && wave <= endWave)
		{
			time += g_ETime;

			if (time >= 60.0f)
			{
				time = 0.0f;

				for (int i = 0; i < waveBrave.size(); i++)
				{
					if (waveBrave.at(i)->key == to_string(wave) + "wave")
					{
						Character* ctr;

						ctr = CHARACTERMANAGER->AddBrave("01", GENDER_NONE, waveBrave.at(i)->braveName);
						ctr->SetPosition(D3DXVECTOR2(-100.0f, 170.0f));
						ctr->SetTime(10.0f + (i * 3.0f));
					}
				}
				soundTime = 10.0f;
				wave++;
			}
		}
	}
}

void Quest::Release()
{
	btn->Release();

	waveBrave.clear();
	waveBrave.shrink_to_fit();
}

void Quest::Render(D3DXVECTOR2 pos)
{
	btn->SetImgName("kanban_header");
	
	btn->SetPosition(pos);
	btn->SetImgScale(D3DXVECTOR2(300.0f, 70.0f));
	btn->SetColBoxScale(D3DXVECTOR2(300.0f, 70.0f));
	btn->Update();

	if (isClear == QUEST_NONE)
		btn->RenderFrame(0, 0);
	if (isClear == QUEST_PROGRESS)
		btn->RenderFrame(0, 1);
	if (isClear == QUEST_CLEAR)
		btn->RenderFrame(0, 2);

	int height = WINSIZEY / 2;

	RECT frc = {(LONG)pos.x - 130, WINSIZEY - (LONG)pos.y - 23, WINSIZEX, WINSIZEY};
	WRITEMANAGER->RenderText(questName, frc, 24.0f, L"µ¸¿òÃ¼", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

	if (PtInRect(btn->GetRect(), g_ptMouse))
	{
		btn->SetImgName("kanban_paper");
		btn->SetImgPosition(D3DXVECTOR2(pos.x + 400.0f, height));
		btn->SetImgScale(D3DXVECTOR2(400.0f, 380.0f));
		btn->Render();

		frc = { (LONG)(pos.x + 400.0f) - 60, height, WINSIZEX, height - 146 };
		WRITEMANAGER->RenderText(questName, frc, 16.0f, L"µ¸¿òÃ¼", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

		frc = { (LONG)(pos.x + 400.0f) - 60, height - 50, WINSIZEX, height - 96 };
		WRITEMANAGER->RenderText(questInfo, frc, 16.0f, L"µ¸¿òÃ¼", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

		btn->SetImgName("star");
		btn->SetImgPosition(D3DXVECTOR2(pos.x + 440.0f, height - 65.0f));
		btn->SetImgScale(D3DXVECTOR2(210.0f, 30.0f));
		btn->RenderFrame(0, questDif);

		frc = { (LONG)(pos.x + 400.0f) - 60, height + 110, WINSIZEX, height + 108 };
		WRITEMANAGER->RenderText(to_wstring(questGold) + L"G", frc, 20.0f, L"±¼¸²Ã¼", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

		if (KEYMANAGER->isKeyDown(VK_LBUTTON))
			isOrder = true;
	}
}

void Quest::Clear()
{
	wave = 1;
	isOrder = false;
	isClear = QUEST_CLEAR;
	playerGold += questGold;
	questGold = 0;
	time = 7.0f;
}
