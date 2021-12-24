#pragma once

struct Furniture
{
	Button* currentFur;
	int frameX, frameY;
	vector<Button*> allFurniture;
	vector<int> furniPrice;
	wstring name;

	Furniture(D3DXVECTOR2 frame, D3DXVECTOR2 pos)
	{
		currentFur = new Button("furni_icon", pos, D3DXVECTOR2(50.0f, 50.0f));
		this->frameX = (int)frame.x;
		this->frameY = (int)frame.y;
	}

	void Render()
	{
		currentFur->RenderFrame(frameX, frameY);
	}

	void Release()
	{
		for (int i =0; i<allFurniture.size(); i++)
		{
			allFurniture.at(i)->Release();
		}

		currentFur->Release();

		allFurniture.clear();
		allFurniture.shrink_to_fit();

		furniPrice.clear();
		furniPrice.shrink_to_fit();
	}
};

class RoomInfo
{
private:
	D3DXVECTOR2 select;
	string name;
	int roomNum;
	int monthGold;
	float time;

	Rect* rc;
	Image* img;
	Image* liveMonster;
	Furniture* choiceFurni;
	vector<Furniture*> furni;
	Button* GoldPM;
	 
public:
	RoomInfo();
	~RoomInfo();

	void Init();
	void ClickFurni();
	void IsChangeFurni();
	void release();
	void update();
	void render();
	void Price();
	void MonthGoldPM();	//월세 올리고 내리기
	void KickLiveMon();

	void SetMonthGold(int gold) { monthGold = gold; }
	void SetRoomNum(int num) { roomNum = num; }
	void SetLiveMonster(Character* monster)
	{
		liveMonster = IMGMANAGER->GetImage(monster->GetTribeName());
		name = monster->GetName();
	}
	bool GetIsChoiceFurni()
	{
		if (choiceFurni == nullptr)
			return false;
		return true;
	}
	
	int GetMonthGold() { return monthGold; }
	int GetRoomNum() { return roomNum; }
	Image* GetLiveMonster() { return liveMonster; }
};

