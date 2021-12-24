#include "stdafx.h"
#include "KingRoom.h"


KingRoom::KingRoom(string name, D3DXVECTOR2 pos, D3DXVECTOR2 scale, int step)
{
	kingDoor = new Button(name, pos, scale);
	posX = pos.x;
	this->step = step;
	inKingRoom[0] = false;
	inKingRoom[1] = false;
	rc = new Rect();
	color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	upgradePrice.emplace_back(1500);
	upgradePrice.emplace_back(10000);
	upgradePrice.emplace_back(50000);
}


KingRoom::~KingRoom()
{
}

void KingRoom::Release()
{
	kingDoor->Release();
	SAFE_DELETE(rc);
}

void KingRoom::Update()
{
	rc->Position(WINSIZEX / 10.0f * 2.7f, WINSIZEY / 10 * 8 - 15.0f);
	rc->Scale(200.0f, 30.0f);
	rc->Update();

	kingDoor->Update();

	if (kingDoor->IsKeyDown(VK_LBUTTON))
	{
		inKingRoom[0] = true;
	}

	if (inKingRoom[0])
	{
		if (KEYMANAGER->isKeyDown(VK_RBUTTON))
		{
			if(inKingRoom[1])
				inKingRoom[1] = false;
			else
				inKingRoom[0] = false;
		}
	}
}

void KingRoom::Render()
{
	if (step > 1)
	{
		for (int i = 0; i < CHARACTERMANAGER->GetAllBrave().size(); i++)
		{
			if (CHARACTERMANAGER->GetAllBrave().at(i)->GetPos().x == kingDoor->GetColBoxPosition().x &&
				CHARACTERMANAGER->GetAllBrave().at(i)->GetColor().a > 0.0f && CHARACTERMANAGER->GetAllBrave().at(i)->GetColor().a < 1.0f)
			{
				kingDoor->SetImgName("apart" + to_string(step) + "_doorOpenM");
				kingDoor->SetImgPosition(D3DXVECTOR2(posX + kingDoor->GetColBoxScale().x - 3.0f, kingDoor->GetImgPosition().y));
			}
			else
			{
				kingDoor->SetImgName("apart" + to_string(step) + "_doorCloseM");
				kingDoor->SetImgPosition(D3DXVECTOR2(posX, kingDoor->GetImgPosition().y));
			}
		}

		kingDoor->Render();
	}
}

void KingRoom::KingRoomInside(int step)
{
	if (inKingRoom[0])
	{
		img = IMGMANAGER->GetImage("room_window");
		img->Scale(WINSIZEX / 10.0f * 8.0f, WINSIZEY);
		img->Position(WINSIZEX / 2.0f, WINSIZEY / 2.0f);
		img->Render();

		RECT frc = { WINSIZEX / 10 * 2 - 80, WINSIZEY / 10 * 1, WINSIZEX,  WINSIZEY };
		WRITEMANAGER->RenderText(L"마왕실", frc, 30.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

		frc = { WINSIZEX / 10 * 2, WINSIZEY / 10 * 2, WINSIZEX,  WINSIZEY };
		WRITEMANAGER->RenderText(L"아파트 증축하기", frc, 24.0f, L"돋움체", color, DWRITE_FONT_WEIGHT_HEAVY);

		if (PtInRect(rc->GetRect(), g_ptMouse) && !inKingRoom[1])
		{
			color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			if (KEYMANAGER->isKeyDown(VK_LBUTTON))
			{
				if(step < 3)
					inKingRoom[1] = true;
			}
		}
		else
			color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		if (inKingRoom[1] && step < 3)
		{
			img = IMGMANAGER->GetImage("piyomaster");
			img->Scale(200.0f, 200.0f);
			img->Position(WINSIZEX / 10 * 6.5f, WINSIZEY / 2.0f - 50.0f);
			img->Render();

			img = IMGMANAGER->GetImage("room_fukidashi");
			img->Scale(350.0f, 170.0f);
			img->Position(WINSIZEX / 10 * 6.7f, WINSIZEY / 10.0f * 6.5f);
			img->Render();

			frc = { WINSIZEX / 10 * 6 - 50, WINSIZEY / 10 * 3 - 20, WINSIZEX,  WINSIZEY };
			WRITEMANAGER->RenderText(L"다음 아파트 증축에는\n" + to_wstring(upgradePrice.at(step)) + L"G가 필요피요!", frc, 24.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_NORMAL);

			frc = { WINSIZEX / 10 * 7 - ((LONG)to_wstring(playerGold).size() * 12) + 160, WINSIZEY / 10, WINSIZEX,  WINSIZEY };
			WRITEMANAGER->RenderText(to_wstring(playerGold) + L"G", frc, 24.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

			frc = { WINSIZEX / 10 * 7 - 60, WINSIZEY / 10 * 1 + 30, WINSIZEX,  WINSIZEY };
			WRITEMANAGER->RenderText(L"증축비용", frc, 24.0f, L"돋움체",
				D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

			frc = { WINSIZEX / 10 * 7 - ((LONG)to_wstring(upgradePrice.at(step)).size() * 12) + 160, WINSIZEY / 10 * 1 + 30, WINSIZEX,  WINSIZEY };
			WRITEMANAGER->RenderText(to_wstring(upgradePrice.at(step)) + L"G", frc, 24.0f, L"돋움체",
				D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);
		}
	}
}
