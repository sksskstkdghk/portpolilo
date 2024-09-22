#pragma once
class KingRoom
{
private:
	Image* img;
	Rect* rc;
	Button* kingDoor;
	D3DXCOLOR color;
	float posX;
	int step;

	bool inKingRoom[2];
	vector<int> upgradePrice;

public:
	KingRoom(string name, D3DXVECTOR2 pos, D3DXVECTOR2 scale, int step);
	~KingRoom();

	void SetStep(int step) { this->step = step; }
	void SetPosX(float posX) { this->posX = posX; }

	int GetStep() { return step; }
	float GetPosX() { return posX; }
	Button& GetKingDoor() { return *kingDoor; }
	int GetUpgradePrice() { return upgradePrice.at(step - 1); }

	void Release();
	void Update();
	void Render();

	void KingRoomInside(int step);
	bool* InKingRoom() { return inKingRoom; }
};

