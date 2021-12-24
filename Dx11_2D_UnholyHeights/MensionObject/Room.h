#pragma once
#include "RoomInfo.h"

class Room
{
private:
	Button * button;
	RoomInfo* room;
	Image* MonsterPicture;
	string apartName, liveMonsterName;
	float doorX;

	int roomStep;

	bool isRoomInfo;
	bool doorCon;

	float kickTime = 0.0f;

public:
	Room(string name, D3DXVECTOR2 pos, D3DXVECTOR2 scale);
	Room(string name, D3DXVECTOR2 pos, D3DXVECTOR2 imgScale, D3DXVECTOR2 colBoxScale);
	~Room();

	void SetLiveMonster(Character* monster)
	{ 
		MonsterPicture = IMGMANAGER->GetImage(monster->GetTribeName());
		liveMonsterName = monster->GetName();
		monster->SetHome(button->GetColBoxPosition().x, roomStep);

		room->SetLiveMonster(monster);
	}
	void SetRoomStep(int step) { roomStep = step; }
	void SetRoomInfo(bool room) { isRoomInfo = room; }
	void SetDoorCon(bool con) { doorCon = con; }
	void SetApartName(string name) { apartName = name; }
	void SetDoorX(float X) { doorX = X; }

	int GetRoomStep() { return roomStep; }
	Button& GetButton() { return *button; }
	RoomInfo& GetRoom() { return *room; }
	bool GetIsRoomInfo() { return isRoomInfo; }
	bool GetDoorCon() { return doorCon; }
	string GetMonsterName() { return liveMonsterName; }

	void Release();
	void Update();
	void Render();

	void openDoor();
};

