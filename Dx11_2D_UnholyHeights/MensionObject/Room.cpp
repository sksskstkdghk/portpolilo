#include "stdafx.h"
#include "Room.h"


Room::Room(string name, D3DXVECTOR2 pos, D3DXVECTOR2 scale)
{
	room = new RoomInfo();
	apartName = name;
	button = new Button(name + "_doorClose", pos, scale);
	MonsterPicture = nullptr;
	doorX = pos.x;

	doorCon = false;
	liveMonsterName = "";
}


Room::Room(string name, D3DXVECTOR2 pos, D3DXVECTOR2 imgScale, D3DXVECTOR2 colBoxScale)
{
	room = new RoomInfo();
	apartName = name;
	button = new Button(name + "_doorClose", pos, imgScale, colBoxScale);
	isRoomInfo = false;
	MonsterPicture = nullptr;
	doorX = pos.x;

	doorCon = false;
}

Room::~Room()
{
}

void Room::Release()
{
	SAFE_DELETE(room);
	button->Release();
}

void Room::Update()
{
	button->Update();

	if (MonsterPicture != nullptr)
	{
		openDoor();

		if (CHARACTERMANAGER->GetMonster(liveMonsterName)->GetState() == (int)STATE_KICK ||
			CHARACTERMANAGER->GetMonster(liveMonsterName)->GetCurrentHP() <= 0)
		{
			liveMonsterName = "";
			MonsterPicture = nullptr;
		}

		if (room->GetLiveMonster() == nullptr)
		{
			kickTime += g_ETime;

			if (kickTime >= 0.5f)
			{
				CHARACTERMANAGER->GetMonster(liveMonsterName)->Kick();
				kickTime = 0.0f;
			}
		}
	}
	
	//room->update();
}

void Room::Render()
{
	if (doorCon)
		button->SetImgName(apartName + "_doorOpen");

	if(!doorCon)
		button->SetImgName(apartName + "_doorClose");

	button->Render();

	if (MonsterPicture != nullptr && !doorCon)
	{
		MonsterPicture->Position(D3DXVECTOR2(button->GetImgPosition().x, button->GetImgPosition().y + 13.0f));
		MonsterPicture->Scale(D3DXVECTOR2(80.0f * 0.35f, 67.2f * 0.4f));
		MonsterPicture->Color(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
		MonsterPicture->Rotate(0.0f, 0.0f, 0.0f);
		MonsterPicture->Render(0.13f, 0.075f, 0.37f, 0.20f);
	}
}

void Room::openDoor()
{
	if (CHARACTERMANAGER->GetMonster(liveMonsterName)->GetState() == (int)STATE_IN_HOME ||
		CHARACTERMANAGER->GetMonster(liveMonsterName)->GetState() == (int)STATE_OUT_HOME)
	{
		button->SetImgPosition(D3DXVECTOR2(doorX + button->GetImgScale().x - 8.0f, button->GetImgPosition().y));
		doorCon = true;
	}
	else
	{
		button->SetImgPosition(D3DXVECTOR2(doorX, button->GetImgPosition().y));
		doorCon = false;
	}
}