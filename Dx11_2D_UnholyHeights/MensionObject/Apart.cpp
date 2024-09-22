#include "stdafx.h"
#include "Apart.h"


Apart::Apart()
{

	
}


Apart::~Apart()
{
}

HRESULT Apart::init()
{
	step = (int)APART01;
	apartHeight = 2;

	ApartName = "apart" + to_string(apartHeight - 1);

	roukaStartPos = 123.0f;
	roukaScale = 30.0f;
	apartPosX = 70.0f;
	roukaPosY = 100.0f;

	isRoomRender = false;

	for (int i = 0; i < 16; i++)
	{
		room.emplace_back(new Room(ApartName, D3DXVECTOR2(678.0f + (107.0f * (i % 4)), 167.0f + (roukaPosY * (i / 4))), D3DXVECTOR2(45.0f, 70.0f)));
		room.at(i)->GetButton().SetImgScale(D3DXVECTOR2(55.0f, 80.0f));
		room.at(i)->GetRoom().SetRoomNum((i / 4 + 1) * 100 + (i % 4 + 1));
		room.at(i)->SetRoomStep(i / 4);
	}
	for (int i = 0; i < 4; i++)
	{
		if (i % 2 == 0)
		{
			upFloor.emplace_back(D3DXVECTOR2(1100.0f, 170.0f + roukaPosY * i));
			downFloor.emplace_back(D3DXVECTOR2(1100.0f, roukaPosY * (i + 1) + 170.0f));
		}
		else
		{
			upFloor.emplace_back(D3DXVECTOR2(600.0f, 170.0f + roukaPosY * i));
			downFloor.emplace_back(D3DXVECTOR2(600.0f, roukaPosY * (i + 1) + 170.0f));
		}
	}

	kingDoor = new KingRoom(ApartName + "_doorClose", D3DXVECTOR2(722.0f, 250.0f), D3DXVECTOR2(35.0f, 35.0f), step + 1);

	return S_OK;
}

void Apart::release()
{
	for (int i =0; i<room.size(); i++)
	{
		room.at(i)->Release();
	}

	room.clear();
	room.shrink_to_fit();
}

void Apart::update()
{
	//일반 방 클릭
	MonRoomClick();
}

void Apart::render()
{
	img = IMGMANAGER->GetImage(ApartName);
	img->Scale(D3DXVECTOR2(WINSIZEX / 2.0f, 300.0f + 95.0f * step));
	img->Position(D3DXVECTOR2(WINSIZEX / 7 * 5 - apartPosX, 120.0f));
	img->Color(backColor);
	img->Render();

	for (int i = 0; i < (step + 1) * 4; i++)
	{
		room.at(i)->Render();
	}

	kingDoor->Render();
}

void Apart::RoukaRender()
{
	for (int i = 0; i < apartHeight; i++)
	{
		img = IMGMANAGER->GetImage(ApartName + "_rouka");
		img->Scale(D3DXVECTOR2(WINSIZEX / 2.0f - 20.0f, roukaScale));
		img->Position(D3DXVECTOR2(WINSIZEX / 7 * 5 - 70, roukaStartPos + (roukaPosY * i)));
		img->Color(backColor);
		img->Render();
	}
}

void Apart::RoomRender()
{
	for (int i = 0; i < room.size(); i++)
	{
		if (room.at(i)->GetIsRoomInfo())
			room.at(i)->GetRoom().render();
	}
}

//월세 입금
void Apart::MonthGoldSupply()
{
	for (int i = 0; i < (step + 1) * 4; i++)
	{
		if (room.at(i)->GetMonsterName() != "")
		{
			playerGold += room.at(i)->GetRoom().GetMonthGold();
		}
	}
}

void Apart::IsUpgrade()
{
	if (kingDoor->InKingRoom()[1])
	{
		if (KEYMANAGER->isKeyDown(VK_LBUTTON))
		{
			if (playerGold >= kingDoor->GetUpgradePrice())
			{
				playerGold -= kingDoor->GetUpgradePrice();

				kingDoor->InKingRoom()[1] = false;
				kingDoor->InKingRoom()[0] = false;

				UpgradeApart();
			}
		}
	}
}

void Apart::MonRoomClick()
{
	for (int i = 0; i < (step + 1) * 4; i++)
	{
		if(!isRoomRender)
			room.at(i)->Update();

		//현재 보이는 방
		if (room.at(i)->GetButton().IsKeyDown(VK_LBUTTON))
		{
			if (CHARACTERMANAGER->GetAllBrave().size() > 0 &&
				room.at(i)->GetMonsterName() != "" &&
				CHARACTERMANAGER->GetMonster(room.at(i)->GetMonsterName())->GetPos().x == CHARACTERMANAGER->GetMonster(room.at(i)->GetMonsterName())->GetHome().doorPosX)
			{
				CHARACTERMANAGER->GetMonster(room.at(i)->GetMonsterName())->SetState(STATE_OUT_HOME);
			}
			else if (!isRoomRender && CHARACTERMANAGER->GetAllBrave().size() <= 0)
			{
				isRoomRender = true;
				room.at(i)->SetRoomInfo(isRoomRender);
			}
		}

		//방 인포창 닫기
		if (isRoomRender && !room.at(i)->GetRoom().GetIsChoiceFurni() &&
			room.at(i)->GetIsRoomInfo())
		{
			if (KEYMANAGER->isKeyDown(VK_RBUTTON))
			{
				isRoomRender = false;
				room.at(i)->SetRoomInfo(isRoomRender);
			}

		}
	}
}

void Apart::ApartCurrentStep()
{
	if (step == (int)APART01)
	{
		roukaStartPos = 123.0f;
		roukaScale = 30.0f;
		apartPosX = 70.0f;
		roukaPosY = 100.0f;
	}
	else if (step == (int)APART02)
	{
		roukaStartPos = 130.0f;
		roukaScale = 30.0f;
		apartPosX = 88.0f;
		roukaPosY = 95.0f;

		kingDoor->GetKingDoor().SetImgScale(D3DXVECTOR2(55.0f, 80.0f));
		kingDoor->GetKingDoor().SetColBoxScale(D3DXVECTOR2(45.0f, 70.0f));
		kingDoor->GetKingDoor().SetPosition(D3DXVECTOR2(955.0f, 358.0f));

		for (int i = 0; i < 4; i++)
		{
			if (i % 2 == 0)
			{
				upFloor.at(i).x = 1100.0f;
				downFloor.at(i).x = 1100.0f;
			}
			else
			{
				upFloor.at(i).x = 585.0f;
				downFloor.at(i).x = 585.0f;
			}
		}
	}
	else if (step == (int)APART03)
	{
		roukaStartPos = 137.0f;
		roukaScale = 40.0f;
		apartPosX = 80.0f;
		roukaPosY = 95.0f;

		kingDoor->GetKingDoor().SetPosition(D3DXVECTOR2(710.0f, 450.0f));

		for (int i = 0; i < 4; i++)
		{
			if (i % 2 == 0)
			{
				upFloor.at(i).x = 1093.0f;
				downFloor.at(i).x = 1093.0f;
			}
			else
			{
				upFloor.at(i).x = 588.0f;
				downFloor.at(i).x = 588.0f;
			}
		}
	}
	else if (step == (int)APART04)
	{
		roukaStartPos = 140.0f;
		roukaScale = 50.0f;
		apartPosX = 85.0f;
		roukaPosY = 101.0f;

		kingDoor->GetKingDoor().SetPosition(D3DXVECTOR2(828.0f, 575.0f));

		for (int i = 0; i < 4; i++)
		{
			if (i % 2 == 0)
			{
				upFloor.at(i).x = 1105.0f;
				downFloor.at(i).x = 1105.0f;
			}
			else
			{
				upFloor.at(i).x = 585.0f;
				downFloor.at(i).x = 585.0f;
			}
		}
	}

	kingDoor->SetPosX(kingDoor->GetKingDoor().GetImgPosition().x);
}

void Apart::UpgradeApart()
{
	step++;	//아파트 층수 증가

	ApartCurrentStep();	//층수에 맞게 세부 위치 설정
	apartHeight = 2 + step;	//아파트 높이 변경
	ApartName = "apart" + to_string(apartHeight - 1);	//아파트 공사
	kingDoor->GetKingDoor().SetImgName("apart" + to_string(step + 1) + "_doorCloseM");
	kingDoor->SetStep(step + 1);

	//계단 세부 변경
	for (int i = 0; i < 4; i++)
	{
		upFloor.at(i).y = 170 + roukaPosY * i;
		downFloor.at(i).y = 170 + roukaPosY * (i + 1);
	}

	//문 세부 변경
	for (int i = 0; i < room.size(); i++)
	{
		room.at(i)->SetApartName(ApartName);

		if (step == 2)
		{
			room.at(i)->GetButton().SetPosition(D3DXVECTOR2(678.0f + (105.0f * (i % 4)), 167.0f + (roukaPosY * (i / 4))));

		}
		else
		{
			room.at(i)->GetButton().SetPosition(D3DXVECTOR2(678.0f + (107.0f * (i % 4)), 167.0f + (roukaPosY * (i / 4))));

		}

		if (step == 3)
		{
			room.at(i)->GetButton().SetPosition(D3DXVECTOR2(678.0f + (107.0f * (i % 4)), 173.0f + (roukaPosY * (i / 4))));
			room.at(i)->GetButton().SetColBoxScale(D3DXVECTOR2(45.0f, 74.0f));
			room.at(i)->GetButton().SetImgScale(D3DXVECTOR2(55.0f, 84.0f));
		}

		room.at(i)->SetDoorX(room.at(i)->GetButton().GetImgPosition().x);
	}
}
